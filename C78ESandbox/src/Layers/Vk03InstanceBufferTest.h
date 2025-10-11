#pragma once
#include <C78E.h>

using namespace C78E; // not great, but im lazy

class VkInstanceBufferTest : public C78E::Layer {
public:
    struct UniformBufferTestObject {
        float animTime = 0.f;
        alignas(16) mat4 view;
        alignas(16) mat4 proj;

        static UniformLayout getUniformLayout() {
            UniformLayout layout;
            layout.pushAttribute(UniformField("animTime", PrimitiveType::Float32, 1));
            layout.pushAttribute(UniformField("view", PrimitiveType::Float32, 16));
            layout.pushAttribute(UniformField("proj", PrimitiveType::Float32, 16));
            return layout;
        }
    };
public:
    VkInstanceBufferTest(C78E::Window& window)
        : Layer(window, "VkInstanceBufferTest") {
    }
    virtual ~VkInstanceBufferTest() = default;

    void onAttach() override {

        const FilePath appDirectory = FileSystem::C78RootDirectory / "C78ESandbox/";
        const FilePath shaderCache = appDirectory / "assets/cache/shaders/";
        const FilePath shaderDirectory = appDirectory / "assets/shaders/";
        const FilePath vkTestShader = shaderDirectory / "vk03InstanceBufferTest.glsl";

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

        {   // InstanceBuffer Setup
            const InstanceBufferLayout bufferLayout{
                VertexAttribute("i_Position", PrimitiveType::Float32, 2)
            };
            const float vertices[] = {
                -.6f, -.6f,
                +.6f, -.6f,
                +.6f, +.6f,
                -.6f, +.6f,
            };
            const size_t instanceCount = 4;
            const size_t bufferByteSize = bufferLayout.getStride() * instanceCount;

            //  Vertex StagingBuffer - Device Host coherent memory
            m_InstanceStagingBuffer = StagingBuffer::create(ctx, bufferByteSize);
            m_InstanceStagingBuffer->map();
            std::memcpy(m_InstanceStagingBuffer->data(), vertices, bufferByteSize);

            m_InstanceBuffer = VertexBuffer::create(ctx, VertexInputRate::Instance, bufferLayout, m_InstanceStagingBuffer);

            // add to Vertex Input Description
            m_PipelineLayout->setInstanceBufferLayout(0, bufferLayout);
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
            m_PipelineLayout->setVertexBufferLayout(0, bufferLayout);
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

        {
            m_CameraTransform.setTranslation(vec3(0.f, 0.f, -1.5f));

            updateUBO(Timestep());
            m_PipelineLayout->setUniformBufferLayout(ShaderStages::Vertex | ShaderStages::Fragment, 0, UniformBufferTestObject::getUniformLayout());
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

        C78E_INFO("VkInstanceBufferTest attached!");
    }

    void onDetach() override {
        for (Ref<CommandBuffer>& cb : m_SwapChainCommands) {
            cb = nullptr;
        }

        C78E_INFO("VkInstanceBufferTest detached!");
    }

    void onUpdate(C78E::Timestep delta) override {
        GraphicsContext& ctx = m_Window.getGraphicsContext();

        vec3 camTranslation = m_CameraTransform.getTranslation();
        if (m_Window.isKeyPressed(Input::Key::A)) {
            camTranslation += vec3(-1.f, 0.f, 0.f) * delta.getSeconds();
        }
        if (m_Window.isKeyPressed(Input::Key::D)) {
            camTranslation += vec3(+1.f, 0.f, 0.f) * delta.getSeconds();
        }
        if (m_Window.isKeyPressed(Input::Key::W)) {
            camTranslation += vec3(0.f, -1.f, 0.f) * delta.getSeconds();
        }
        if (m_Window.isKeyPressed(Input::Key::S)) {
            camTranslation += vec3(0.f, +1.f, 0.f) * delta.getSeconds();
        }
        if (m_Window.isKeyPressed(Input::Key::Up)) {
            camTranslation += vec3(0.f, 0.f, +1.f) * delta.getSeconds();
        }
        if (m_Window.isKeyPressed(Input::Key::Down)) {
            camTranslation += vec3(0.f, 0.f, -1.f) * delta.getSeconds();
        }
        m_CameraTransform.setTranslation(camTranslation);

        updateUBO(delta);

        // Render
        uint32_t frameIndex = ctx.beginFrame();

        ctx.submit(frameIndex, m_SwapChainCommands[frameIndex]);

        ctx.endFrame(frameIndex);
    }

    void onEvent(C78E::Event& e) override {
        C78E::EventDispatcher dispatcher(e);
        dispatcher.dispatch<C78E::KeyPressedEvent>(C78E_BIND_THIS_METHOD(VkInstanceBufferTest::onKeyPressed));
        dispatcher.dispatch<C78E::MouseButtonPressedEvent>(C78E_BIND_THIS_METHOD(VkInstanceBufferTest::onMouseButtonPressed));
        dispatcher.dispatch<C78E::WindowResizeEvent>(C78E_BIND_THIS_METHOD(VkInstanceBufferTest::onWindowResize));
        dispatcher.dispatch<C78E::MouseMovedEvent>(C78E_BIND_THIS_METHOD(VkInstanceBufferTest::onMouseMoved));
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

    void onMouseMoved(C78E::MouseMovedEvent e) {

        if (m_Window.isMousePressed(Input::Mouse::ButtonLeft)) {
            WindowSize size = m_Window.getSize();
            const float sens = 0.5f;

            const vec2 mousePos = { e.getX(), e.getY() };
            const vec2 deltaMousePos = mousePos - m_MousePos;
            m_MousePos = mousePos;

            const float dx = (+1.f * deltaMousePos.x / static_cast<float>(size.x));
            const float dy = (-1.f * deltaMousePos.y / static_cast<float>(size.y));

            const vec3 rotate = sens * vec3(dy, dx, 0.f);

            vec3 rotation = m_CameraTransform.getRotation().toEulerAngles();
            rotation = glm::mod(rotation + rotate, glm::two_pi<scalar>());
            m_CameraTransform.setRotation(rotation);

            e.handled = true;
        }

    }

    void onMouseButtonPressed(C78E::MouseButtonPressedEvent e) {

        if (e.getMouseButton() == Input::Mouse::ButtonLeft) {
            m_MousePos = m_Window.getMousePositionFromWindowOriginInPixels();
        }

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

    void updateUBO(Timestep deltaTime) {
        if (!m_UniformBuffer) {
            GraphicsContext& ctx = m_Window.getGraphicsContext();
            m_UniformBuffer = UniformBuffer::create(ctx, UniformBufferTestObject::getUniformLayout());

            UniformBufferTestObject& ubo = m_UniformBuffer->mapAs<UniformBufferTestObject>();
            ubo.animTime = 0;

        }
        UniformBufferTestObject& ubo = m_UniformBuffer->mapAs<UniformBufferTestObject>(); // should not remap anything -> keep mapped Mem
        ubo.animTime += deltaTime.getSeconds();
        ubo.proj = glm::perspectiveLH(glm::radians(75.f), (float)m_Window.getWidth() / m_Window.getHeight(), 0.001f, 1000.f);
        ubo.view = m_CameraTransform.toInvMat();
        m_UniformBuffer->writeDeviceMemory();
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
            cmd->bind(m_InstanceBuffer);
            cmd->bind(m_VertexBuffer);
            cmd->bind(m_IndexBuffer);

            cmd->bind(m_UniformBuffer);

            cmd->setRenderArea(swapChain.getFullRenderArea());
            cmd->drawIndices(m_IndexBuffer->getIndexCount(), 4);

            cmd->endRenderPass();
            cmd->endRecording();

            m_SwapChainCommands.push_back(cmd);
        }
    }

private:
    Ref<RenderPass> m_RenderPass;

    Ref<StagingBuffer> m_InstanceStagingBuffer;
    Ref<VertexBuffer> m_InstanceBuffer;
    Ref<StagingBuffer> m_VertexStagingBuffer;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<StagingBuffer> m_IndexStagingBuffer;
    Ref<IndexBuffer> m_IndexBuffer;

    Ref<GraphicsPipelineLayout> m_PipelineLayout;
    Ref<GraphicsPipelineConfig> m_PipelineConfig;
    Ref<GraphicsPipeline> m_Pipeline;

    Ref<UniformBuffer> m_UniformBuffer;

    std::vector<Ref<CommandBuffer>> m_SwapChainCommands;

    Transform3D m_CameraTransform;

    vec2 m_MousePos = {};
};
