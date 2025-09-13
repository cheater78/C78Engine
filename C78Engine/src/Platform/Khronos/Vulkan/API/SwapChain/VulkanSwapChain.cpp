#include "C78EPCH.h"
#include "VulkanSwapChain.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanFrameBuffer.h>

namespace C78E {

    VkPresentModeKHR toVkPresentModeKHR(RefreshMode refreshMode) {
        switch (refreshMode) {
        case RefreshMode::Unlimited:
            return VK_PRESENT_MODE_IMMEDIATE_KHR;
        case RefreshMode::Vsync:
            return VK_PRESENT_MODE_MAILBOX_KHR;
        default:
            return VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    VkColorSpaceKHR toVkColorSpaceKHR(ColorSpace colorSpace) {
        switch (colorSpace) {
        case ColorSpace::sRGB:
            return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        case ColorSpace::sRGBLinear:
            return VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
        default:
            return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        }
    }

    Ref<VulkanSwapChain> VulkanSwapChain::create(GraphicsContext& ctx, const SwapChainConfig& config) {
        return createRef<VulkanSwapChain>(ctx, config);
    }

    VulkanSwapChain::VulkanSwapChain(GraphicsContext& ctx, const SwapChainConfig& config)
        : SwapChain(ctx, config) {
        VulkanGraphicsContext& vulkanGraphicsContext = ctx.getAs<VulkanGraphicsContext>();
        m_Device = vulkanGraphicsContext.getDevice();
		m_Surface = vulkanGraphicsContext.getSurface();

		C78E_CORE_ASSERT(createSwapChain(), "VulkanSwapChain::VulkanSwapChain: Failed to create swap chain!");

        //TODO: move
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkResult resultIAS = vkCreateSemaphore(m_Device->getVkDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore);
        VkResult resultRFS = vkCreateSemaphore(m_Device->getVkDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore);
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        VkResult resultIFF = vkCreateFence(m_Device->getVkDevice(), &fenceInfo, nullptr, &m_InFlightFence);
    }

    VulkanSwapChain::~VulkanSwapChain() {
		destroySwapChain();

        //TODO: move
        vkDestroySemaphore(m_Device->getVkDevice(), m_ImageAvailableSemaphore, nullptr);
        vkDestroySemaphore(m_Device->getVkDevice(), m_RenderFinishedSemaphore, nullptr);
        vkDestroyFence(m_Device->getVkDevice(), m_InFlightFence, nullptr);
    }

    bool VulkanSwapChain::recreate(SwapChainConfig config) {
		m_Config = config;
		createSwapChain();
        return false;
    }
    bool VulkanSwapChain::resize(ImageSize size) {
		m_Config.swapChainElementFrameBufferSpec.size = size;
        createSwapChain();
        return false;
    }

    bool VulkanSwapChain::nextFrame() {
        const SwapChainBufferCount bufferMode = m_Config.bufferCount;

        uint32_t nextImageIndex;

        VkResult swapResult = vkAcquireNextImageKHR(
            m_Device->getVkDevice(),
            m_VkSwapChain,
            std::numeric_limits<uint64_t>::max(),
            m_ImageAvailableSemaphore,
            VK_NULL_HANDLE,
            &nextImageIndex
        );
        C78E_CORE_SOFT_VALIDATE(swapResult == VK_SUCCESS, "VulkanSwapChain::nextFrame: Aquiring Next Image was suboptimal!");
        C78E_CORE_VALIDATE(swapResult == VK_SUCCESS || swapResult == VK_SUBOPTIMAL_KHR, return false, "VulkanSwapChain::nextFrame: Failed to acquire next image!");

        return true;
    }

    Ref<FrameBuffer> VulkanSwapChain::createSwapChainFrameBuffer(ImageIndex swapChainImageIndex, Ref<RenderPass> renderPass) {
        return createRef<VulkanFrameBuffer>(m_GraphicsContext, m_Config.swapChainElementFrameBufferSpec, renderPass, this, swapChainImageIndex, m_VkImages[swapChainImageIndex]);
    }

    bool VulkanSwapChain::createSwapChain() {
        C78E_CORE_TRACE("VulkanSwapChain::createSwapChain: Creating swap chain...");

        VulkanGraphicsContext& ctx = m_GraphicsContext.getAs<VulkanGraphicsContext>();
        
        // For recreation
        const VkSwapchainKHR previousSwapChain = m_PreviousVkSwapChain; // copy previous swap chain handle for recreation call
        if (m_VkSwapChain != VK_NULL_HANDLE) { // case of recreation
            m_PreviousVkSwapChain = m_VkSwapChain; // store previous swap chain handle
        }

		const SwapChainConfig& swapChainConfig = m_Config; // copy config for recreation
        const VkSurfaceCapabilitiesKHR& capabilities = ctx.getSurfaceCapabilities();
        const std::vector<VkSurfaceFormatKHR>& supportedFormats = ctx.getSurfaceFormats();
        const std::vector<VkPresentModeKHR>& supportedPresentModes = ctx.getSurfacePresentModes();

        const FrameBufferAttachmentSpecification& swapChainColorSpec =
            m_Config.swapChainElementFrameBufferSpec.colorAttachmentSpecifications[m_Config.swapChainColorAttachmentIndex];

        const VkFormat requestedNativeFormat = toVkFormat(swapChainColorSpec.format);
        VkSurfaceFormatKHR surfaceFormat;
        bool requestedFormatSupported = false;
        for (const VkSurfaceFormatKHR& availableFormat : supportedFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && //TODO: placeholder
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                surfaceFormat = availableFormat;
                requestedFormatSupported = true;
                break;
            }
        }
        C78E_CORE_VALIDATE(requestedFormatSupported, return false, "VulkanSwapChain::createSwapChain: Requested Format not supported!");

        VkPresentModeKHR requestedNativePresentMode = toVkPresentModeKHR(m_Config.refreshMode);
        const bool requestedPresentMode = std::find(supportedPresentModes.begin(), supportedPresentModes.end(), requestedNativePresentMode) != supportedPresentModes.end();
        C78E_CORE_VALIDATE(requestedPresentMode, return false, "VulkanSwapChain::createSwapChain: Requested Present Mode not supported!");

        VkExtent2D extent;
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            extent = capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = getVulkanSwapChainExtent();
            actualExtent.width = std::max(
                capabilities.minImageExtent.width,
                std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(
                capabilities.minImageExtent.height,
                std::min(capabilities.maxImageExtent.height, actualExtent.height));
            extent = actualExtent;
        }

		const uint32_t requestedImageCount = static_cast<uint32_t>(m_Config.bufferCount); // requested image count from config
		C78E_CORE_VALIDATE(requestedImageCount > 0, return false, "VulkanSwapChain::createSwapChain: Frame count must be greater than 0!");
        C78E_CORE_VALIDATE(requestedImageCount >= capabilities.minImageCount, return false, "VulkanSwapChain::createSwapChain: Frame count is less than minimum image count!");
		C78E_CORE_VALIDATE(requestedImageCount <= capabilities.maxImageCount, return false, "VulkanSwapChain::createSwapChain: Frame count exceeds maximum image count!");

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;

        createInfo.surface = m_Surface;
        createInfo.minImageCount = requestedImageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //TODO: convert from config
        
        uint32_t universalQueueFamilyIndex = m_Device->getUniversalQueueFamilyIndex();

        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 1;
        createInfo.pQueueFamilyIndices = &universalQueueFamilyIndex;

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = requestedNativePresentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = previousSwapChain;
        
		VkResult createSwapChainResult = vkCreateSwapchainKHR(m_Device->getVkDevice(), &createInfo, nullptr, &m_VkSwapChain);
		C78E_CORE_VALIDATE(createSwapChainResult == VK_SUCCESS, return false, "Failed to create swap chain!");

        uint32_t imageCount;
		VkResult queryImageCountResult = vkGetSwapchainImagesKHR(m_Device->getVkDevice(), m_VkSwapChain, &imageCount, nullptr);
		C78E_CORE_VALIDATE(queryImageCountResult == VK_SUCCESS, return false, "Failed to query swap chain image count!");

		m_VkImages.resize(imageCount);
        VkResult queryImagesResult = vkGetSwapchainImagesKHR(m_Device->getVkDevice(), m_VkSwapChain, &imageCount, m_VkImages.data());
		C78E_CORE_VALIDATE(queryImagesResult == VK_SUCCESS, return false, "Failed to query swap chain images!");
        
		return true;
    }

    /**
	 * @brief Permanently destroys the swap chain. Not used for recreation.
     */
    void VulkanSwapChain::destroySwapChain() {
        if (m_VkSwapChain != nullptr) {
            vkDestroySwapchainKHR(m_Device->getVkDevice(), m_VkSwapChain, nullptr);
            m_VkSwapChain = nullptr;
        }
    }

    VkExtent2D VulkanSwapChain::getVulkanSwapChainExtent() const {
        return VkExtent2D{
            m_Config.swapChainElementFrameBufferSpec.size.x,
            m_Config.swapChainElementFrameBufferSpec.size.y
        };
    }


} // namespace C78E
