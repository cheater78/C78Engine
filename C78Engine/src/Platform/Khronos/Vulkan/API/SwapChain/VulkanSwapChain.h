#pragma once
#include <C78E/Graphics/API/SwapChain/SwapChain.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanFrameBuffer.h>

namespace C78E {

    VkPresentModeKHR toVkPresentModeKHR(RefreshMode refreshMode);
    VkColorSpaceKHR toVkColorSpaceKHR(ColorSpace colorSpace);

    class VulkanSwapChain : public SwapChain {
    public:
        VulkanSwapChain(GraphicsContext& ctx, const SwapChainConfig& config);
        ~VulkanSwapChain();

        virtual bool recreate(SwapChainConfig config) override;
        virtual bool resize(ImageSize size) override;

        virtual Ref<FrameBuffer> aquireNextFramebuffer(uint32_t frameIndex) override;
        virtual Ref<FrameBuffer> getFrameBuffer(uint32_t frameIndex) override;
    public:
		VkSwapchainKHR getVkSwapChain() const;
        
        VkSemaphore getImageAvailableSemaphore(uint32_t frameIndex);
        VkSemaphore getFinishedRenderSemaphore(uint32_t frameIndex);
        VkFence getInFlightFence(uint32_t frameIndex);

        uint32_t waitForAnyFence();

    protected:
		bool createSwapChain(); // Creation of the swap chain, suitable for recreation
		void destroySwapChain(); // Permanent destruction of the swap chain, not suitable for recreation

        bool createFrameBuffers(Ref<RenderPass> renderPass);

        VkExtent2D getVulkanSwapChainExtent() const;
    // protected:
        // GraphicsContext& m_GraphicsContext;
        // SwapChainConfig m_Config;
    protected:
        Ref<VulkanDevice> m_Device;
        VkSurfaceKHR m_Surface;

        VkSwapchainKHR m_VkSwapChain = VK_NULL_HANDLE;
        VkSwapchainKHR m_PreviousVkSwapChain = VK_NULL_HANDLE; // for recreation

		std::vector<Ref<VulkanFrameBuffer>> m_FrameBuffers;

        // InFlight Sync Objects - managed by VulkanSwapChain, borrowed by FrameBuffer
        std::vector<VkSemaphore> m_FrameImageAvailableSemaphores;
        std::vector<VkSemaphore> m_FrameFinishedRenderSemaphores;
        std::vector<VkFence> m_FrameInFlightFences;
        

    };

} // namespace C78E
