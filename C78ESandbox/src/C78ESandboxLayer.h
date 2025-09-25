#pragma once
#include <C78E.h>

using namespace C78E; // not great, but im lazy

class C78ESandboxLayer : public C78E::Layer {

private:
    struct SwapChainCommand;
public:
    C78ESandboxLayer(C78E::Window& window)
        : Layer(window, "C78ESandboxLayer") {

    }
	virtual ~C78ESandboxLayer() = default;

    void onAttach() override {

        const FilePath shaderCache = FileSystem::C78RootDirectory / "assets/cache/shaders/";
        const FilePath shaderDirectory = FileSystem::C78RootDirectory / "assets/shaders/";
        const FilePath vkTestShader = shaderDirectory / "vkTestShader.glsl";

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
		m_PipelineLayout = createRef<VulkanGraphicsPipelineLayout>(ctx);

        for (const auto& [stage, shader] : shaders) {
            m_PipelineLayout->setShader(stage, shader);
        }

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
        for (SwapChainCommand& sc : m_SwapChainCommands) {
            sc.commandBuffer = nullptr;
        }

        C78E_INFO("C78ESandboxLayer detached!");
    }

    void onUpdate(C78E::Timestep delta) override {
		GraphicsContext& ctx = m_Window.getGraphicsContext();

        // Render
        uint32_t frameIndex = ctx.beginFrame();

        ctx.submit(frameIndex, m_SwapChainCommands[frameIndex].commandBuffer);

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
            SwapChainCommand cmd;
            cmd.commandBuffer = ctx.createCommandBuffer();

            cmd.commandBuffer->beginRecording();

            cmd.commandBuffer->beginRenderPass(m_RenderPass, swapChain.getFrameBuffer(i));
            cmd.commandBuffer->bindPipeline(m_Pipeline);
            cmd.commandBuffer->setRenderArea(swapChain.getFullRenderArea());
            cmd.commandBuffer->drawVertecies(3);
            cmd.commandBuffer->endRenderPass();

            cmd.commandBuffer->endRecording();

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

	Ref<GraphicsPipelineLayout> m_PipelineLayout;
	Ref<GraphicsPipelineConfig> m_PipelineConfig;
	Ref<GraphicsPipeline> m_Pipeline;

    struct SwapChainCommand {
        Ref<CommandBuffer> commandBuffer;
	};

	std::vector<SwapChainCommand> m_SwapChainCommands;
    
};
