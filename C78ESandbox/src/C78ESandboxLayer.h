#pragma once
#include <C78E.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanIndexBuffer.h>

using namespace C78E; // not great, but im lazy

class C78ESandboxLayer : public C78E::Layer {
public:
    C78ESandboxLayer(C78E::Window& window)
        : Layer(window, "C78ESandboxLayer") {

    }
	virtual ~C78ESandboxLayer() = default;

    void onAttach() override {

        const FilePath shaderCache = FileSystem::C78EngineDirectory / "assets/cache/shaders/";
        const FilePath shaderDirectory = FileSystem::C78EngineDirectory / "assets/shaders/";
        const FilePath vkTestShader = shaderDirectory / "vkVertexBufferTest.glsl";

        GraphicsContext& ctx = m_Window.getGraphicsContext();
        Ref<ShaderManager> shaderManager = ctx.createShaderManager(shaderCache);
        LoadedFileShaders shaders = shaderManager->loadShaderFromSourceFile(vkTestShader);

        m_RenderPass = RenderPass::create(ctx); //TODO: currently empty RenderPass

        SwapChainConfig swapChainConfig{
            .frameBufferRenderPass = m_RenderPass,
            .frameBufferCount = (uint32_t)SwapChainBufferCount::TripleBuffer,
            .frameBufferAttachmentSpecifications = { /* no additional Attachments */},
            // no depth
            // no multiSample
            .swapInterval = SwapInterval::Immediate
        };

        SwapChain& swapChain = ctx.createSwapChain(swapChainConfig);

		m_PipelineConfig = createRef<VulkanGraphicsPipelineConfig>();
        m_PipelineConfig->testDepthBuffer = false;

        // Pipeline Layout
		m_PipelineLayout = createRef<VulkanGraphicsPipelineLayout>(ctx);
        for (const auto& [stage, shader] : shaders) {
            m_PipelineLayout->setShader(stage, shader);
        }

        {   // VertexBuffer Setup
            const VertexBufferLayout bufferLayout{
                VertexAttribute("v_Position", PrimitiveType::Float32, 2),
                VertexAttribute("v_Color", PrimitiveType::Float32, 3)
            };
            const float vertices[] = {
                -.5f, -.5f, 1.f, 0.f, 0.f,
                +.5f, -.5f, 0.f, 1.f, 0.f,
                +.5f, +.5f, 0.f, 0.f, 1.f,
                -.5f, +.5f, 1.f, 1.f, 1.f,
            };
            const size_t vertexCount = 4;
            const size_t bufferByteSize = bufferLayout.getStride() * vertexCount;

            //  Vertex StagingBuffer - Device Host coherent memory
            m_VertexStagingBuffer = StagingBuffer::create(ctx, bufferByteSize);
            m_VertexStagingBuffer->map();
            std::memcpy(m_VertexStagingBuffer->data(), vertices, bufferByteSize);

            m_VertexBuffer = VertexBuffer::create(ctx, VertexInputRate::Vertex, bufferLayout, m_VertexStagingBuffer);

            // add to Vertex Input Description
            m_PipelineLayout->addVertexBufferLayout(bufferLayout);
        }

        {   //  IndexBuffer
            const IndexLayout indexLayout = IndexLayout::UInt32;
            const uint32_t indices[] = {
                0, 1, 2, 2, 3, 0
            };
            const size_t indexCount = 6;
            const size_t indexBufferSize = indexLayout.size() * indexCount;

            //  Index StagingBuffer - Device Host coherent memory
            m_IndexStagingBuffer = StagingBuffer::create(ctx, indexBufferSize);
            m_IndexStagingBuffer->map();
            std::memcpy(m_IndexStagingBuffer->data(), indices, indexBufferSize);

            m_IndexBuffer = IndexBuffer::create(ctx, indexLayout, m_IndexStagingBuffer);
        }

        // Pipeline Setup
        m_Pipeline = GraphicsPipeline::create(
            ctx,
            m_PipelineLayout,
            m_PipelineConfig,
            swapChain.getFullRenderArea(),
            m_RenderPass,
            0);

        recordCommandbuffers();

        C78E_INFO("C78ESandboxLayer attached!");
    }

    void onDetach() override {
        for (Ref<CommandBuffer>& cb : m_SwapChainCommands) {
            cb = nullptr;
        }

        C78E_INFO("C78ESandboxLayer detached!");
    }

    void onUpdate(C78E::Timestep delta) override {
		GraphicsContext& ctx = m_Window.getGraphicsContext();

        // Render
        uint32_t frameIndex = ctx.beginFrame();

        ctx.submit(frameIndex, m_SwapChainCommands[frameIndex]);

        ctx.endFrame(frameIndex);
    }

    void onEvent(C78E::Event& e) override {
        C78E::EventDispatcher dispatcher(e);
        dispatcher.dispatch<C78E::KeyPressedEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onKeyPressed));
        dispatcher.dispatch<C78E::MouseButtonPressedEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onMouseButtonPressed));
        dispatcher.dispatch<C78E::WindowResizeEvent>(C78E_BIND_THIS_METHOD(C78ESandboxLayer::onWindowResize));
    }

    void onKeyPressed(C78E::KeyPressedEvent e) {
        if (e.getKeyCode() == C78E::Input::Key::F11) {
            m_Window.setWindowMode((m_Window.getWindowMode() == C78E::WindowMode::Windowed) ? C78E::WindowMode::FullScreen : C78E::WindowMode::Windowed);
            e.handled = true;
        }

        if (e.getKeyCode() == C78E::Input::Key::F1) {
            GraphicsContext& ctx = m_Window.getGraphicsContext();
            SwapChain& swc = ctx.getSwapChain();
            
            m_Window.setSize({ 2048, 1280 });
            e.handled = true;
        }
        if (e.getKeyCode() == C78E::Input::Key::F2) {
            GraphicsContext& ctx = m_Window.getGraphicsContext();
            SwapChain& swc = ctx.getSwapChain();

            m_Window.setSize({ 1960, 1080 });
            e.handled = true;
        }

    }

    bool onMouseButtonPressed(C78E::MouseButtonPressedEvent e) {
        return false;
    }

    void onWindowResize(C78E::WindowResizeEvent e) {
        GraphicsContext& ctx = m_Window.getGraphicsContext();
        SwapChain& swc = ctx.getSwapChain();
        C78E_TRACE("Triggering SwapChain resize! new Dim {}", e.getSize());
        swc.recreate();
        recordCommandbuffers();
    }

    virtual void onDebugRender() override {

    }

    void recordCommandbuffers() {
        m_SwapChainCommands.clear();
        GraphicsContext& ctx = m_Window.getGraphicsContext();
        SwapChain& swapChain = ctx.getSwapChain();

        for (uint32_t i = 0; i < swapChain.getFrameCount(); i++) {
            Ref<CommandBuffer> cmd = ctx.createCommandBuffer();

            cmd->beginRecording();
            cmd->beginRenderPass(m_RenderPass, swapChain.getFrameBuffer(i));

            cmd->bindPipeline(m_Pipeline);
            cmd->bind(m_VertexBuffer);
            cmd->bind(m_IndexBuffer);
            
            cmd->setRenderArea(swapChain.getFullRenderArea());

            //cmd->drawVertices(3);
            cmd->drawIndices(m_IndexBuffer->getIndexCount());

            cmd->endRenderPass();
            cmd->endRecording();

            m_SwapChainCommands.push_back(cmd);
        }
    }

    void recordCommandbuffersHelloTriangle() {
        m_SwapChainCommands.clear();
        GraphicsContext& ctx = m_Window.getGraphicsContext();
        SwapChain& swapChain = ctx.getSwapChain();

        for (uint32_t i = 0; i < swapChain.getFrameCount(); i++) {
            Ref<CommandBuffer> cmd = ctx.createCommandBuffer();

            cmd->beginRecording();

            cmd->beginRenderPass(m_RenderPass, swapChain.getFrameBuffer(i));
            cmd->bindPipeline(m_Pipeline);
            cmd->setRenderArea(swapChain.getFullRenderArea());
            cmd->drawVertices(3);
            cmd->endRenderPass();

            cmd->endRecording();

            m_SwapChainCommands.push_back(cmd);
        }
    }

private:
    // Order of operations:
    // 1. Begin recording
    // (2. Optionally) bind a offscreen framebuffer
    // 3. Begin render pass, either by creating a new one, providing a framebuffer or using an existing render pass
    // 4. Bind a pipeline
    // 5. Bind Shader elements (vertex buffers, index buffers, uniform buffers, storage buffers, storage images)
    //   this entails the correct Descriptors
    // 6. Draw calls (indexed or not)
    // (7. Optionally) Bind another pipeline, and repeat steps
    // 7. End render pass
    // (8. Optionally) bind a different framebuffer or start a new render pass, then repeat steps
    // 8. End recording
	bool setupTestScene() {

		return true;
	}

private:
    Ref<RenderPass> m_RenderPass;

    Ref<StagingBuffer> m_VertexStagingBuffer;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<StagingBuffer> m_IndexStagingBuffer;
    Ref<IndexBuffer> m_IndexBuffer;

	Ref<GraphicsPipelineLayout> m_PipelineLayout;
	Ref<GraphicsPipelineConfig> m_PipelineConfig;
	Ref<GraphicsPipeline> m_Pipeline;

	std::vector<Ref<CommandBuffer>> m_SwapChainCommands;
    
};
