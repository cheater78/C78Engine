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

        GraphicsContext& ctx = m_Window.getGraphicsContext();
        Ref<ShaderManager> shaderManager = ctx.createShaderManager(shaderCache);

        const FilePath vkTestShader = FileSystem::C78RootDirectory / "assets/shaders/vkTestShader.glsl";

        LoadedFileShaders shaders = shaderManager->loadShaderFromSourceFile(vkTestShader);

        SwapChainConfig swapChainConfig;
        swapChainConfig.bufferCount = SwapChainBufferCount::TripleBuffer;
        swapChainConfig.refreshMode = RefreshMode::Vsync;
		swapChainConfig.swapChainColorAttachmentIndex = 0;

		FrameBufferSpecification fbSpec;
        fbSpec.hasDepthAttachment = false;
		fbSpec.swapChainColorAttachmentIndex = 0; // TODO: speced twice, bad
		fbSpec.samples = 1;
		fbSpec.size = m_Window.getSize();

        FrameBufferAttachmentSpecification fbAttachSpec{ ImageFormat::RGBA8 };
        fbSpec.colorAttachmentSpecifications = { fbAttachSpec };

        swapChainConfig.swapChainElementFrameBufferSpec = fbSpec;

		m_SwapChain = ctx.createSwapChain(swapChainConfig);

		m_RenderPass = RenderPass::create(ctx);
        
		m_PipelineConfig = createRef<GraphicsPipelineConfig>();
		m_PipelineLayout = createRef<GraphicsPipelineLayout>();

		GraphicsPipelineTarget gpt;
        gpt.renderPass = m_RenderPass;
        gpt.subpassIndex = 0;
        gpt.renderAreaOffset = {0, 0 };
        gpt.renderAreaSize = m_SwapChain->getConfig().swapChainElementFrameBufferSpec.size;
        gpt.scissorOffset = {0, 0 };
        gpt.scissorSize = m_SwapChain->getConfig().swapChainElementFrameBufferSpec.size;

		m_Pipeline = GraphicsPipeline::create(ctx, m_PipelineLayout, m_PipelineConfig, gpt);

        for (uint32_t i = 0; i < m_SwapChain->frameCount(); i++) {
            SwapChainCommand cmd;
            cmd.commandBuffer = ctx.createCommandBuffer();
			cmd.frameBuffer = m_SwapChain->createSwapChainFrameBuffer(i, m_RenderPass);

            cmd.commandBuffer->beginRecording();
            cmd.commandBuffer->beginRenderPass(m_RenderPass, cmd.frameBuffer);

            cmd.commandBuffer->bindPipeline(m_Pipeline);

			cmd.commandBuffer->drawVertecies(3);

			cmd.commandBuffer->endRenderPass();
			cmd.commandBuffer->endRecording();
        }

        C78E_INFO("C78ESandboxLayer attached!");
    }

    void onDetach() override {
        C78E_INFO("C78ESandboxLayer detached!");
    }

    void onUpdate(C78E::Timestep delta) override {


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
    }

    bool onMouseButtonPressed(C78E::MouseButtonPressedEvent e) {
        return false;
    }

    void onWindowResize(C78E::WindowResizeEvent e) {
        //m_Window.getGraphicsContext().resize(e.getSize());
    }

    virtual void onDebugRender() override {

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
	Ref<SwapChain> m_SwapChain;
    
    Ref<RenderPass> m_RenderPass;

	Ref<GraphicsPipelineLayout> m_PipelineLayout;
	Ref<GraphicsPipelineConfig> m_PipelineConfig;
	Ref<GraphicsPipeline> m_Pipeline;

    struct SwapChainCommand {
        Ref<CommandBuffer> commandBuffer;
        Ref<FrameBuffer> frameBuffer;
	};

	std::vector<SwapChainCommand> m_SwapChainCommands;
    
};
