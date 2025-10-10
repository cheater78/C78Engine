#pragma once
#include <C78E/Graphics/API/SwapChain/SwapChain.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>
#include <Platform/Khronos/Vulkan/Core/VulkanConversions.h>
#include <Platform/Khronos/Vulkan/API/Image/VulkanFrameBuffer.h>

namespace C78E {

    class VulkanSwapChain : public SwapChain {
    public:
        VulkanSwapChain(GraphicsContext& ctx, const SwapChainConfig& cfg);
        ~VulkanSwapChain();

        virtual bool recreate() override;
        virtual bool recreate(SwapChainConfig cfg) override;

        virtual Ref<FrameBuffer> aquireNextFramebuffer(uint32_t frameIndex) override;
        virtual Ref<FrameBuffer> getFrameBuffer(uint32_t frameIndex) override;
    public:
		VkSwapchainKHR getVkSwapChain() const;
        
        VkSemaphore getImageAvailableSemaphore(uint32_t frameIndex);
        VkSemaphore getFinishedRenderSemaphore(uint32_t frameIndex);
        VkFence getInFlightFence(uint32_t frameIndex);

        uint32_t waitForAnyFence();

    protected:
        void createState(const SwapChainConfig& cfg);

		bool createSwapChain(); // Creation of the swap chain, suitable for recreation
		void destroySwapChain(); // Permanent destruction of the swap chain, not suitable for recreation

        bool createSyncObjects();
        void destroySyncObjects();

        bool createFrameBuffers();

    protected:
        Ref<VulkanDevice> m_Device;
        VkSurfaceKHR m_Surface;

        VkSwapchainKHR m_VkSwapChain = VK_NULL_HANDLE;
        VkSwapchainKHR m_VkPreviousSwapChain = VK_NULL_HANDLE;

		std::vector<Ref<VulkanFrameBuffer>> m_FrameBuffers; //TODO: these have to be owned! 

        // InFlight Sync Objects - managed by VulkanSwapChain, borrowed by FrameBuffer
        std::vector<VkSemaphore> m_FrameImageAvailableSemaphores;
        std::vector<VkSemaphore> m_FrameFinishedRenderSemaphores;
        std::vector<VkFence> m_FrameInFlightFences;
        

    };

    ImageSize acquireSupportedSurfaceSize(const VkSurfaceCapabilitiesKHR& capabilities);
    VkSurfaceFormatKHR acquireSupportedSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& supportedFormats, ImageFormat requestedFormat = ImageFormat::ABGR8, ColorSpace colorSpace = ColorSpace::sRGB);
    SwapInterval acquireSupportedSurfaceSwapInterval(const std::vector<VkPresentModeKHR>& supportedPresentModes, const SwapInterval requestedSwapInterval);


} // namespace C78E
