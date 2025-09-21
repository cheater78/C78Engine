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

        m_RenderPass = RenderPass::create(ctx);

        SwapChainConfig swapChainConfig;
        swapChainConfig.bufferCount = (uint32_t)SwapChainBufferCount::TripleBuffer;
        swapChainConfig.refreshMode = RefreshMode::Vsync;
		swapChainConfig.swapChainColorAttachmentIndex = 0;
        swapChainConfig.renderPass = m_RenderPass;

		FrameBufferSpecification fbSpec;
        fbSpec.hasDepthAttachment = false;
		fbSpec.swapChainColorAttachmentIndex = 0; // TODO: specd twice, bad
		fbSpec.samples = 1;
		fbSpec.size = m_Window.getSize();

        FrameBufferAttachmentSpecification fbAttachSpec{ ImageFormat::BGRA8S };
        fbSpec.colorAttachmentSpecifications = { fbAttachSpec };

        swapChainConfig.swapChainElementFrameBufferSpec = fbSpec;

        SwapChain& swapChain = ctx.createSwapChain(swapChainConfig);

		m_PipelineConfig = createRef<VulkanGraphicsPipelineConfig>();
		m_PipelineLayout = createRef<VulkanGraphicsPipelineLayout>(ctx);

        for (const auto& [stage, shader] : shaders) {
            m_PipelineLayout->setShader(stage, shader);
        }

		GraphicsPipelineTarget gpt;
        gpt.renderPass = m_RenderPass;
        gpt.subpassIndex = 0;
        gpt.renderAreaOffset = {0, 0 };
        gpt.renderAreaSize = swapChain.getConfig().swapChainElementFrameBufferSpec.size;
        gpt.scissorOffset = {0, 0 };
        gpt.scissorSize = swapChain.getConfig().swapChainElementFrameBufferSpec.size;

		m_Pipeline = GraphicsPipeline::create(ctx, m_PipelineLayout, m_PipelineConfig, gpt);

        for (uint32_t i = 0; i < swapChain.getFrameCount(); i++) {
            SwapChainCommand cmd;
            cmd.commandBuffer = ctx.createCommandBuffer();
            cmd.frameBuffer = swapChain.getFrameBuffer(i);

            cmd.commandBuffer->beginRecording();


            cmd.commandBuffer->beginRenderPass(m_RenderPass, cmd.frameBuffer);

            cmd.commandBuffer->bindPipeline(m_Pipeline);
            
			{ // TODO: temp vulkan specific code, move to vulkan command buffer class
                Ref<VulkanCommandBuffer> vkCmd = castRef<VulkanCommandBuffer>(cmd.commandBuffer);

                VkViewport viewport{};
                viewport.x = static_cast<float>(gpt.renderAreaOffset.x);
                viewport.y = static_cast<float>(gpt.renderAreaOffset.y);
                viewport.width = static_cast<float>(gpt.renderAreaSize.x);
                viewport.height = static_cast<float>(gpt.renderAreaSize.y);
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;
                vkCmdSetViewport(vkCmd->getVkCommandBuffer(), 0, 1, &viewport);

                VkRect2D scissor{};
                scissor.offset = { static_cast<int32_t>(gpt.scissorOffset.x), static_cast<int32_t>(gpt.scissorOffset.y) };
                scissor.extent = { static_cast<uint32_t>(gpt.scissorSize.x), static_cast<uint32_t>(gpt.scissorSize.y) };
                vkCmdSetScissor(vkCmd->getVkCommandBuffer(), 0, 1, &scissor);

            }

			cmd.commandBuffer->drawVertecies(3);


            cmd.commandBuffer->endRenderPass();

			cmd.commandBuffer->endRecording();

			m_SwapChainCommands.push_back(cmd);
        }

        C78E_INFO("C78ESandboxLayer attached!");
    }

    void onDetach() override {
        for (SwapChainCommand& sc : m_SwapChainCommands) {
            sc.commandBuffer = nullptr;
            sc.frameBuffer = nullptr;
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
