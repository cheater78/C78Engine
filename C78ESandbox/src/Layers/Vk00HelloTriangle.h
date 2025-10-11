#pragma once
#include <C78E.h>

using namespace C78E; // not great, but im lazy

class VkHelloTriangle : public C78E::Layer {
public:
    VkHelloTriangle(C78E::Window& window)
        : Layer(window, "VkHelloTriangle") {
    }
    virtual ~VkHelloTriangle() = default;

    void onAttach() override {

        const FilePath appDirectory = FileSystem::C78RootDirectory / "C78ESandbox/";
        const FilePath shaderCache = appDirectory / "assets/cache/shaders/";
        const FilePath shaderDirectory = appDirectory / "assets/shaders/";
        const FilePath vkTestShader = shaderDirectory / "vk00HelloTriangle.glsl";

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

        // Pipeline Setup
        m_Pipeline = GraphicsPipeline::create(
            ctx,
            m_PipelineLayout,
            m_PipelineConfig,
            swapChain.getFullRenderArea(),
            m_RenderPass,
            0);

        recordCommandbuffers();

        C78E_INFO("VkHelloTriangle attached!");
    }

    void onDetach() override {
        for (Ref<CommandBuffer>& cb : m_SwapChainCommands) {
            cb = nullptr;
        }

        C78E_INFO("VkHelloTriangle detached!");
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
        dispatcher.dispatch<C78E::KeyPressedEvent>(C78E_BIND_THIS_METHOD(VkHelloTriangle::onKeyPressed));
        dispatcher.dispatch<C78E::MouseButtonPressedEvent>(C78E_BIND_THIS_METHOD(VkHelloTriangle::onMouseButtonPressed));
        dispatcher.dispatch<C78E::WindowResizeEvent>(C78E_BIND_THIS_METHOD(VkHelloTriangle::onWindowResize));
    }

    void onKeyPressed(C78E::KeyPressedEvent e) {
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
            cmd->setRenderArea(swapChain.getFullRenderArea());
            cmd->drawVertices(3);
            cmd->endRenderPass();

            cmd->endRecording();

            m_SwapChainCommands.push_back(cmd);
        }
    }

private:
    Ref<RenderPass> m_RenderPass;

    Ref<GraphicsPipelineLayout> m_PipelineLayout;
    Ref<GraphicsPipelineConfig> m_PipelineConfig;
    Ref<GraphicsPipeline> m_Pipeline;

    std::vector<Ref<CommandBuffer>> m_SwapChainCommands;

};
