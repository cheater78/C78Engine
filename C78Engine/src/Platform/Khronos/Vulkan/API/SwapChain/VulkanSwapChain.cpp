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

    VulkanSwapChain::VulkanSwapChain(GraphicsContext& ctx, const SwapChainConfig& config)
        : SwapChain(ctx, config) {
        VulkanGraphicsContext& vulkanGraphicsContext = ctx.getAs<VulkanGraphicsContext>();
        m_Device = vulkanGraphicsContext.getDevice();
		m_Surface = vulkanGraphicsContext.getSurface();

		C78E_CORE_ASSERT(createSwapChain(), "VulkanSwapChain::VulkanSwapChain: Failed to create swap chain!");
    }

    VulkanSwapChain::~VulkanSwapChain() {
		destroySwapChain();
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

    Ref<FrameBuffer> VulkanSwapChain::aquireNextFramebuffer(uint32_t frameIndex) {
        constexpr uint64_t waitFenceTimeOut = 30 * 1000000000UL; // in nanoseconds -> seconds
        constexpr uint64_t acquireImageTimeOut = 30 * 1000000000UL; // in nanoseconds -> seconds

        C78E_CORE_VALIDATE(frameIndex < getFrameCount(), return nullptr, "VulkanSwapChain::aquireNextFramebuffer: frameIndex {} requested, not inside of SwapChain frame count!", frameIndex);

        VkResult waitFenceResult = vkWaitForFences(m_Device->getVkDevice(), 1, &m_FrameInFlightFences[frameIndex], VK_TRUE, waitFenceTimeOut);
        C78E_CORE_VALIDATE(waitFenceResult == VK_SUCCESS, return nullptr, "VulkanSwapChain::aquireNextFramebuffer: Waiting for Frame {} failed!", frameIndex);

        VkResult rstFenceResult = vkResetFences(m_Device->getVkDevice(), 1, &m_FrameInFlightFences[frameIndex]);
        C78E_CORE_VALIDATE(rstFenceResult == VK_SUCCESS, return nullptr, "VulkanSwapChain::aquireNextFramebuffer: Resetting Fences for Frame {} failed!", frameIndex);


        uint32_t nextImageIndex = -1;
        VkResult swapResult = vkAcquireNextImageKHR(
            m_Device->getVkDevice(),
            m_VkSwapChain,
            acquireImageTimeOut,
            m_FrameImageAvailableSemaphores[frameIndex],
            VK_NULL_HANDLE,
            &nextImageIndex);
        C78E_CORE_SOFT_VALIDATE(swapResult == VK_SUCCESS, "VulkanSwapChain::aquireNextFramebuffer: Aquiring Next Image was suboptimal!");
        C78E_CORE_VALIDATE(swapResult == VK_SUCCESS || swapResult == VK_SUBOPTIMAL_KHR, return nullptr, "VulkanSwapChain::aquireNextFramebuffer: Failed to acquire next image!");
        C78E_CORE_VALIDATE(nextImageIndex != -1, return nullptr, "VulkanSwapChain::aquireNextFramebuffer: Failed to acquire next image!");

        Ref<VulkanFrameBuffer> vulkanFrameBuffer = m_FrameBuffers[nextImageIndex];
        vulkanFrameBuffer->setImageAvailableSemaphore(m_FrameImageAvailableSemaphores[frameIndex]);
        vulkanFrameBuffer->setRenderFinishedSemaphore(m_FrameFinishedRenderSemaphores[frameIndex]);
        return vulkanFrameBuffer;
    }

    Ref<FrameBuffer> VulkanSwapChain::getFrameBuffer(uint32_t frameIndex) {
        return m_FrameBuffers[frameIndex];
    }

    VkSwapchainKHR VulkanSwapChain::getVkSwapChain() const {
        return m_VkSwapChain;
    }

    VkSemaphore VulkanSwapChain::getImageAvailableSemaphore(uint32_t frameIndex) {
        C78E_CORE_VALIDATE(frameIndex < getFrameCount(), return nullptr, "VulkanSwapChain::getImageAvailableSemaphore: frameIndex {} requested, not inside of SwapChain frame count!", frameIndex);
        return m_FrameImageAvailableSemaphores[frameIndex];
    }

    VkSemaphore VulkanSwapChain::getFinishedRenderSemaphore(uint32_t frameIndex) {
        C78E_CORE_VALIDATE(frameIndex < getFrameCount(), return nullptr, "VulkanSwapChain::getFinishedRenderSemaphore: frameIndex {} requested, not inside of SwapChain frame count!", frameIndex);
        return m_FrameFinishedRenderSemaphores[frameIndex];
    }

    VkFence VulkanSwapChain::getInFlightFence(uint32_t frameIndex) {
        C78E_CORE_VALIDATE(frameIndex < getFrameCount(), return nullptr, "VulkanSwapChain::getInFlightFence: frameIndex {} requested, not inside of SwapChain frame count!", frameIndex);
        return m_FrameInFlightFences[frameIndex];
    }

    uint32_t VulkanSwapChain::waitForAnyFence() {
        VkResult res = vkWaitForFences(m_Device->getVkDevice(), m_FrameInFlightFences.size(), m_FrameInFlightFences.data(), VK_FALSE, 30000000000UL); // 30s
        if (res == VK_SUCCESS) {
            for (uint32_t i = 0; i < m_FrameInFlightFences.size(); i++) {
                if (vkGetFenceStatus(m_Device->getVkDevice(), m_FrameInFlightFences[i]) == VK_SUCCESS) {
                    return i;
                }
            }
        }
        return -1;
    }

    bool VulkanSwapChain::createSwapChain() {
        C78E_CORE_TRACE("VulkanSwapChain::createSwapChain: Creating swap chain...");

        VulkanGraphicsContext& ctx = m_GraphicsContext.getAs<VulkanGraphicsContext>();
        
        // For recreation
        const VkSwapchainKHR previousSwapChain = m_PreviousVkSwapChain; // copy previous swap chain handle for recreation call
        if (m_VkSwapChain != VK_NULL_HANDLE) { // case of recreation
            m_PreviousVkSwapChain = m_VkSwapChain; // store previous swap chain handle

            //TODO: Semaphores and Fences - just reset?
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
            if (availableFormat.format == requestedNativeFormat && 
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

        uint32_t& imageCount = m_Config.bufferCount;
		VkResult queryImageCountResult = vkGetSwapchainImagesKHR(m_Device->getVkDevice(), m_VkSwapChain, &imageCount, nullptr);
		C78E_CORE_VALIDATE(queryImageCountResult == VK_SUCCESS, return false, "Failed to query swap chain image count!");

        const bool createFBresult = createFrameBuffers(m_Config.renderPass);
        C78E_CORE_VALIDATE(createFBresult, return false, "Failed to create VulkanSwapChain FrameBuffers!");

		return true;
    }

    /**
	 * @brief Permanently destroys the swap chain. Not used for recreation.
     */
    void VulkanSwapChain::destroySwapChain() {
        // free FrameBuffers before SwapChain (Handles to FBs belonging to the SwapChain have to be dropped before destroying the SwapChain, EVERYWHERE)
        for (Ref<VulkanFrameBuffer> fb : m_FrameBuffers) {
            fb = nullptr; 
        }
        if (m_VkSwapChain != nullptr) {
            vkDestroySwapchainKHR(m_Device->getVkDevice(), m_VkSwapChain, nullptr);
            m_VkSwapChain = nullptr;
        }
        for (VkSemaphore s : m_FrameImageAvailableSemaphores) {
            if (s != VK_NULL_HANDLE) {
                vkDestroySemaphore(m_Device->getVkDevice(), s, nullptr);
            }
        }
        for (VkSemaphore s : m_FrameFinishedRenderSemaphores) {
            if (s != VK_NULL_HANDLE) {
                vkDestroySemaphore(m_Device->getVkDevice(), s, nullptr);
            }
        }
        for (VkFence f : m_FrameInFlightFences) {
            if (f != VK_NULL_HANDLE) {
                vkDestroyFence(m_Device->getVkDevice(), f, nullptr);
            }
        }

    }

    bool VulkanSwapChain::createFrameBuffers(Ref<RenderPass> renderPass) {
        m_FrameBuffers.clear();

        uint32_t imageCount = getFrameCount();
        m_FrameImageAvailableSemaphores.resize(imageCount);
        m_FrameFinishedRenderSemaphores.resize(imageCount);
        m_FrameInFlightFences.resize(imageCount);
        m_FrameBuffers.resize(imageCount);
        std::vector<VkImage> vkImages(imageCount);

        VkResult queryImagesResult = vkGetSwapchainImagesKHR(m_Device->getVkDevice(), m_VkSwapChain, &imageCount, vkImages.data());
        C78E_CORE_VALIDATE(queryImagesResult == VK_SUCCESS, return false, "Failed to query swap chain images!");

        for (ImageIndex imageIndex = 0; imageIndex < imageCount; imageIndex++) {
            // Create SwapChain FrameBuffer Synchronization - Not tied to the FB below
            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            VkResult resultFIAS = vkCreateSemaphore(m_Device->getVkDevice(), &semaphoreInfo, nullptr, &m_FrameImageAvailableSemaphores[imageIndex]);
            C78E_CORE_VALIDATE(resultFIAS == VK_SUCCESS, return false, "Failed to create FrameImageAvailableSemaphore!");
            VkResult resultFRFS = vkCreateSemaphore(m_Device->getVkDevice(), &semaphoreInfo, nullptr, &m_FrameFinishedRenderSemaphores[imageIndex]);
            C78E_CORE_VALIDATE(resultFRFS == VK_SUCCESS, return false, "Failed to create FrameFinishedRenderSemaphore!");

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            VkResult resultFIFF = vkCreateFence(m_Device->getVkDevice(), &fenceInfo, nullptr, &m_FrameInFlightFences[imageIndex]);
            C78E_CORE_VALIDATE(resultFIFF == VK_SUCCESS, return false, "Failed to create FrameInFlightFence!");
            
            // Create FrameBuffer with SwapChain owned ColorAttachment
            m_FrameBuffers[imageIndex] = createRef<VulkanFrameBuffer>(
                m_GraphicsContext,
                m_Config.swapChainElementFrameBufferSpec,
                renderPass,
                this,
                m_Config.swapChainColorAttachmentIndex, // Color Attachment Index of the FrameBuffer to use the Swap Chain Image for
                vkImages[imageIndex], // Swap Chain Image
                imageIndex // Index of the Image in the Swap Chain
            );
            C78E_CORE_VALIDATE(m_FrameBuffers[imageIndex] != nullptr, return false, "Failed to create VulkanSwapChain FrameBuffer for Image no. {} of {}", imageIndex, imageCount);
        }

        return true;
    }

    VkExtent2D VulkanSwapChain::getVulkanSwapChainExtent() const {
        return VkExtent2D{
            m_Config.swapChainElementFrameBufferSpec.size.x,
            m_Config.swapChainElementFrameBufferSpec.size.y
        };
    }


} // namespace C78E
