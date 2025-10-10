#include "C78EPCH.h"
#include "VulkanSwapChain.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Image/VulkanFrameBuffer.h>

namespace C78E {

    VulkanSwapChain::VulkanSwapChain(GraphicsContext& ctx, const SwapChainConfig& cfg)
        : SwapChain(ctx) {
        createState(cfg); // write cfg to State
        const bool createSwapChainSuccess = createSwapChain();
		C78E_CORE_ASSERT(createSwapChainSuccess, "VulkanSwapChain::VulkanSwapChain: Failed to create SwapChain!");
    }

    VulkanSwapChain::~VulkanSwapChain() {
		destroySwapChain();
    }

    bool VulkanSwapChain::recreate() {
        C78E_CORE_ASSERT(createSwapChain(), "VulkanSwapChain::recreate: Failed to create SwapChain!");
        return false;
    }

    bool VulkanSwapChain::recreate(SwapChainConfig cfg) {
        createState(cfg); // write cfg to State
        const bool createSwapChainSuccess = createSwapChain();
        C78E_CORE_ASSERT(createSwapChainSuccess, "VulkanSwapChain::recreate: Failed to create SwapChain, given the new Config!");
        return false;
    }

    Ref<FrameBuffer> VulkanSwapChain::aquireNextFramebuffer(uint32_t frameIndex) {
        constexpr uint64_t waitFenceTimeOut = 30 * 1000000000ULL; // in nanoseconds -> seconds
        constexpr uint64_t acquireImageTimeOut = 30 * 1000000000ULL; // in nanoseconds -> seconds

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
        if (swapResult == VK_ERROR_OUT_OF_DATE_KHR) {
            createSwapChain();
            return aquireNextFramebuffer(frameIndex);
        }
        C78E_CORE_VALIDATE(swapResult == VK_SUCCESS || swapResult == VK_SUBOPTIMAL_KHR, return nullptr, "failed to acquire swap chain image!");
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

    uint32_t VulkanSwapChain::waitForAnyFence() { //TODO: unused, figure Frame indices out
        VkResult res = vkWaitForFences(m_Device->getVkDevice(), static_cast<uint32_t>(m_FrameInFlightFences.size()), m_FrameInFlightFences.data(), VK_FALSE, 30000000000UL); // 30s
        if (res == VK_SUCCESS) {
            for (uint32_t i = 0; i < m_FrameInFlightFences.size(); i++) {
                if (vkGetFenceStatus(m_Device->getVkDevice(), m_FrameInFlightFences[i]) == VK_SUCCESS) {
                    return i;
                }
            }
        }
        return -1;
    }

    void VulkanSwapChain::createState(const SwapChainConfig& cfg) {
        m_State.frameBufferRenderPass = cfg.frameBufferRenderPass; // stays fixed
        m_State.frameBufferCount = cfg.frameBufferCount; // may changed due to support

        // m_State.frameBufferConfig.size is automatically set during creation, according to surface support

        const size_t frameBufferAttachmentCount = cfg.frameBufferAttachmentSpecifications.size() + 1; // requested Attachents + SwapChain owned Attachment
        constexpr size_t swapChainOwnedAttachmentIndex = 0; // the way attachments are written, the owned one is always first
        m_State.frameBufferConfig.attachmentSpecifications.resize(frameBufferAttachmentCount);

        // m_State.frameBufferConfig.attachmentSpecifications[swapChainOwnedAttachmentIndex] will be handled during creation
        std::copy(cfg.frameBufferAttachmentSpecifications.begin(), cfg.frameBufferAttachmentSpecifications.end(),
            m_State.frameBufferConfig.attachmentSpecifications.begin() + 1); // append all requested Attachemnts
        
        m_State.frameBufferConfig.activeDepthAttachment = cfg.frameBufferActiveDepthAttachment;
        m_State.frameBufferConfig.multiSample = cfg.frameBufferMultiSample;

        m_State.ownedAttachmentIndex = swapChainOwnedAttachmentIndex;
        m_State.swapInterval = cfg.swapInterval;
    }

    /**
     * @brief Creates or recreates the SwapChain, works entirely on m_State for seemless recreation,
     * for first time creation write m_State beforehand, on creation State is always handled as request and checked against support
     * @return true, if the creation was successful
     */
    bool VulkanSwapChain::createSwapChain() {
        C78E_CORE_TRACE("VulkanSwapChain::createSwapChain: Creating swap chain...");
        VulkanGraphicsContext& ctx = m_GraphicsContext.getAs<VulkanGraphicsContext>();
        m_Device = ctx.getDevice();
        m_Surface = ctx.getSurface();
        
        // acquire current SwapChains Target Size and Format, and supported SwapIntervals
        const VkSurfaceCapabilitiesKHR& capabilities = ctx.getSurfaceCapabilities();
        const std::vector<VkSurfaceFormatKHR>& supportedFormats = ctx.getSurfaceFormats();
        const std::vector<VkPresentModeKHR>& supportedPresentModes = ctx.getSurfacePresentModes();

        // acquire (new) SwapChain properties
        // Image Size - automatically read from Surface Capabilities
        const ImageSize imageSize = acquireSupportedSurfaceSize(capabilities);
        const VkExtent2D imageExtent = toVkExtent(imageSize);
        C78E_CORE_TRACE("VulkanSwapChain::createSwapChain: ImageSize: {}", imageSize);

        // Image Count - automatically adjusted by Surface Capabilities
        const uint32_t requestedImageCount = m_State.frameBufferCount;
        const uint32_t imageCount = std::clamp(requestedImageCount, capabilities.minImageCount, capabilities.maxImageCount);
        C78E_CORE_TRACE("VulkanSwapChain::createSwapChain: Requested ImageCount: {}, Chosen Supported Count {}", requestedImageCount, imageCount);
        
        // Surface Format - automatically adjusted by Surface
        const ImageFormat requestedImageFormat = ImageFormat::ABGR8; // TODO: hardcoded, provide a interface to query available Formats, then make it configurable
        const ColorSpace requestedColorSpace = ColorSpace::sRGB; // TODO: hardcoded, provide a interface to query available ColorSpaces, then make it configurable
        const VkSurfaceFormatKHR nativeSurfaceFormat = acquireSupportedSurfaceFormat(supportedFormats, requestedImageFormat, requestedColorSpace);
        const ImageFormat imageFormat = toImageFormat(nativeSurfaceFormat.format);
        const ColorSpace colorSpace = toColorSpace(nativeSurfaceFormat.colorSpace);
        C78E_CORE_TRACE("VulkanSwapChain::createSwapChain: Requested ImageFormat: {}, and ColorSpace: {}, Chosen Supported VkFormat: {}, and VkColorSpaceKHR: {}",
            requestedImageFormat, requestedColorSpace, imageFormat, colorSpace);

        // SwapInterval - automatically adjusted by Surface
        const SwapInterval requestedSwapInterval = m_State.swapInterval;
        const SwapInterval swapInterval = acquireSupportedSurfaceSwapInterval(supportedPresentModes, requestedSwapInterval);
        const VkPresentModeKHR nativeSwapInterval = toVkPresentModeKHR(swapInterval);
        C78E_CORE_TRACE("VulkanSwapChain::createSwapChain: Requested SwapInterval: {}, Chosen Supported VkPresentModeKHR {}", requestedSwapInterval, swapInterval);


        // Clean up (opt.) previous swapchains attributes
        const VkSwapchainKHR obsoleteSwapChain = m_VkPreviousSwapChain;
        const VkSwapchainKHR previousSwapChain = m_VkPreviousSwapChain = m_VkSwapChain;
        const bool recreate = previousSwapChain != VK_NULL_HANDLE;
        if(recreate) {
            m_Device->waitIdle();
            destroySyncObjects();
        }
        
        // Create VkSwapChain
        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext = VK_NULL_HANDLE;
        createInfo.surface = m_Surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = nativeSurfaceFormat.format;
        createInfo.imageColorSpace = nativeSurfaceFormat.colorSpace;
        createInfo.imageExtent = imageExtent;
        createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // TODO: non color SwapChains?
        
        // QueueFamilyIndices - universal does it
        //TODO: extend VulkanDevice to provide all 3 Attribs below depending / or some other interface -> let Device handle that
        const uint32_t universalQueueFamilyIndex = m_Device->getUniversalQueueFamilyIndex();
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 1;
        createInfo.pQueueFamilyIndices = &universalQueueFamilyIndex;

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = nativeSwapInterval;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = previousSwapChain;
        
		const VkResult createSwapChainResult = vkCreateSwapchainKHR(m_Device->getVkDevice(), &createInfo, nullptr, &m_VkSwapChain);
		C78E_CORE_VALIDATE(createSwapChainResult == VK_SUCCESS, return false, "VulkanSwapChain::createSwapChain: Failed to create VkSwapChainKHR!");

        // Update SwapChain State - for fields that could've changed only
        m_State.frameBufferCount = imageCount;
        m_State.frameBufferConfig.size = imageSize;
        m_State.frameBufferConfig.attachmentSpecifications[m_State.ownedAttachmentIndex] = FrameBufferAttachmentSpecification(imageFormat); //TODO: texture option should be configurable
        m_State.swapInterval = swapInterval;

        // query final imageCount and write it to State
        uint32_t& finalImageCount = m_State.frameBufferCount;
        const VkResult queryImageCountResult = vkGetSwapchainImagesKHR(m_Device->getVkDevice(), m_VkSwapChain, &finalImageCount, nullptr);
		C78E_CORE_VALIDATE(queryImageCountResult == VK_SUCCESS, return false, "Failed to query swap chain image count!");

        // Create SyncObjects - not tied to FBs!
        const bool createSyncObjectsResult = createSyncObjects();
        C78E_CORE_VALIDATE(createSyncObjectsResult, return false, "VulkanSwapChain::createSwapChain: Failed to create SyncObjects!");

        // Create SwapChains FrameBuffers based on the current State
        const bool createFrameBuffersResult = createFrameBuffers();
        C78E_CORE_VALIDATE(createFrameBuffersResult, return false, "VulkanSwapChain::createSwapChain: Failed to create FrameBuffers!");

        if (obsoleteSwapChain != VK_NULL_HANDLE) { // Destroy (previous' previous) obsolete SwapChain
            vkDestroySwapchainKHR(m_Device->getVkDevice(), obsoleteSwapChain, nullptr);
        }

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
        }
        if (m_VkPreviousSwapChain != nullptr) {
            vkDestroySwapchainKHR(m_Device->getVkDevice(), m_VkPreviousSwapChain, nullptr);
        }
        destroySyncObjects();

    }

    bool VulkanSwapChain::createSyncObjects() {
        const uint32_t frameCount = getFrameCount();

        m_FrameImageAvailableSemaphores.resize(frameCount);
        m_FrameFinishedRenderSemaphores.resize(frameCount);
        m_FrameInFlightFences.resize(frameCount);

        for (FrameIndex imageIndex = 0; imageIndex < frameCount; imageIndex++) {
            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            VkResult resultFIAS = vkCreateSemaphore(m_Device->getVkDevice(), &semaphoreInfo, nullptr, &m_FrameImageAvailableSemaphores[imageIndex]);
            C78E_CORE_VALIDATE(resultFIAS == VK_SUCCESS, return false, "VulkanSwapChain::createSyncObjects: Failed to create FrameImageAvailableSemaphore!");
            VkResult resultFRFS = vkCreateSemaphore(m_Device->getVkDevice(), &semaphoreInfo, nullptr, &m_FrameFinishedRenderSemaphores[imageIndex]);
            C78E_CORE_VALIDATE(resultFRFS == VK_SUCCESS, return false, "VulkanSwapChain::createSyncObjects: Failed to create FrameFinishedRenderSemaphore!");

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            VkResult resultFIFF = vkCreateFence(m_Device->getVkDevice(), &fenceInfo, nullptr, &m_FrameInFlightFences[imageIndex]);
            C78E_CORE_VALIDATE(resultFIFF == VK_SUCCESS, return false, "VulkanSwapChain::createSyncObjects: Failed to create FrameInFlightFence!");
        }

        return true;
    }
    void VulkanSwapChain::destroySyncObjects() {
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

    bool VulkanSwapChain::createFrameBuffers() {
        m_FrameBuffers.clear();

        uint32_t imageCount = getFrameCount();
        
        m_FrameBuffers.resize(imageCount);
        std::vector<VkImage> vkImages(imageCount);

        VkResult queryImagesResult = vkGetSwapchainImagesKHR(m_Device->getVkDevice(), m_VkSwapChain, &imageCount, vkImages.data());
        C78E_CORE_VALIDATE(queryImagesResult == VK_SUCCESS, return false, "Failed to query swap chain images!");

        for (FrameIndex imageIndex = 0; imageIndex < imageCount; imageIndex++) {
            // Create FrameBuffer with SwapChain owned ColorAttachment
            m_FrameBuffers[imageIndex] = createRef<VulkanFrameBuffer>(
                m_GraphicsContext,
                m_State.frameBufferConfig,
                m_State.frameBufferRenderPass,
                this,
                m_State.ownedAttachmentIndex, // Color Attachment Index of the FrameBuffer to use the Swap Chain Image for
                vkImages[imageIndex], // Swap Chain Image
                imageIndex // Index of the Image in the Swap Chain
            );
            C78E_CORE_VALIDATE(m_FrameBuffers[imageIndex] != nullptr, return false, "Failed to create VulkanSwapChain FrameBuffer for Image no. {} of {}", imageIndex, imageCount);
        }

        return true;
    }

    ImageSize acquireSupportedSurfaceSize(const VkSurfaceCapabilitiesKHR& capabilities) {
        const VkExtent2D& current = capabilities.currentExtent;
        const VkExtent2D& min = capabilities.minImageExtent;
        const VkExtent2D& max = capabilities.maxImageExtent;

        return ImageSize{
            (current.width != std::numeric_limits<uint32_t>::max()) ? current.width : max.width,
            (current.height != std::numeric_limits<uint32_t>::max()) ? current.height : max.height,
        };
    }

    VkSurfaceFormatKHR acquireSupportedSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& supportedFormats, ImageFormat requestedFormat, ColorSpace colorSpace) {
        C78E_CORE_ASSERT(!supportedFormats.empty(), "acquireSupportedSurfaceFormat: supportedFormats was empty!");
        const VkFormat nativeRequestedFormat = toVkFormat(requestedFormat);
        const VkColorSpaceKHR nativeRequestedColorSpace = toVkColorSpaceKHR(colorSpace);

        for (const VkSurfaceFormatKHR& availableFormat : supportedFormats) {
            if (availableFormat.format == nativeRequestedFormat &&
                availableFormat.colorSpace == nativeRequestedColorSpace) {
                return availableFormat;
            }
        }
        C78E_CORE_WARN("acquireSupportedSurfaceFormat: requested Format: {}, and ColorSpace: {}, was not supported! Choosing any other..", requestedFormat, colorSpace);
        // note: we alr established that supportedFormats isn't empty
        return supportedFormats.front();
    }

    SwapInterval acquireSupportedSurfaceSwapInterval(const std::vector<VkPresentModeKHR>& supportedPresentModes, const SwapInterval requestedSwapInterval) {
        C78E_CORE_ASSERT(!supportedPresentModes.empty(), "acquireSupportedSurfaceSwapInterval: supportedPresentModes was empty!");
        const VkPresentModeKHR requestedNativeSwapInterval = toVkPresentModeKHR(requestedSwapInterval);
        const auto it = std::find(supportedPresentModes.begin(), supportedPresentModes.end(), requestedNativeSwapInterval);

        if (it != supportedPresentModes.end()) {
            return requestedSwapInterval; // requested SwapInterval supported
        }
        // requested SwapInterval NOT supported -> search any
        C78E_CORE_WARN("acquireSupportedSurfaceSwapInterval: requested SwapInterval: {}, was not supported! Choosing any other..");
        // note: we alr established that supportedPresentModes isn't empty
        return toSwapInterval(supportedPresentModes.front());
    }

} // namespace C78E
