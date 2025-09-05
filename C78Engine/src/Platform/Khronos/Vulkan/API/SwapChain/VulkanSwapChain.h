#pragma once
#include <C78E/Graphics/API/SwapChain/SwapChain.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanFrameBuffer.h>

namespace C78E {

    VkPresentModeKHR toVkPresentModeKHR(RefreshMode refreshMode);
    VkColorSpaceKHR toVkColorSpaceKHR(ColorSpace colorSpace);

    class VulkanSwapChain : public SwapChain {
    public:
		static Ref<VulkanSwapChain> create(GraphicsContext& ctx, const SwapChainConfig& config);
    public:
        VulkanSwapChain(GraphicsContext& ctx, const SwapChainConfig& config);
        ~VulkanSwapChain();

        virtual bool recreate(SwapChainConfig config) override;
        virtual bool resize(ImageSize size) override;
		virtual bool nextFrame() override;

    public:

    protected:
		bool createSwapChain(); // Creation of the swap chain, suitable for recreation
		void destroySwapChain(); // Permanent destruction of the swap chain, not suitable for recreation

        VkExtent2D getVulkanSwapChainExtent() const;
    protected:
        Ref<VulkanDevice> m_Device;
        VkSurfaceKHR m_Surface;

        VkSwapchainKHR m_VkSwapChain = VK_NULL_HANDLE;
        VkSwapchainKHR m_PreviousVkSwapChain = VK_NULL_HANDLE; // for recreation

        // SwapChain Config
        //  SwapChainConfig m_Config;
        // Swapchain ring buffer
        //  std::vector<Ref<FrameBuffer>> m_FrameBuffers;

        VkSemaphore m_ImageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence m_InFlightFence = VK_NULL_HANDLE;

    };

} // namespace C78E
