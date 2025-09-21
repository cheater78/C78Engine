#include "C78EPCH.h"
#include "vulkan/vulkan_core.h"
#include "VulkanGraphicsContext.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/SwapChain/VulkanSwapChain.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanCommandBuffer.h>

namespace C78E {

    VulkanGraphicsContext::VulkanGraphicsContext(Window& window)
		: GraphicsContext(window) {
		init();
	}
	VulkanGraphicsContext::~VulkanGraphicsContext() {
        shutdown();
	}

    Ref<CommandBuffer> VulkanGraphicsContext::createCommandBuffer() {
		return createRef<VulkanCommandBuffer>(*this, CommandBuffer::Usage::Auto, m_UniversalCommandPool);
    }

    void VulkanGraphicsContext::init() {
		Ref<VulkanGraphicsInstance> vulkanInstance = GraphicsInstance::getAs<VulkanGraphicsInstance>();

        // Create window surface
        VkResult result = glfwCreateWindowSurface(vulkanInstance->getInstance(), m_Window.getNativeWindowAs<GLFWwindow>(), nullptr, &m_VkSurface);
        C78E_CORE_ASSERT(result == VK_SUCCESS, "VulkanGraphicsContext::init: Failed to create window surface!");

		// pick a suitable device
		m_Device = vulkanInstance->pickDevice(m_VkSurface);

        // Create Command Pool
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = m_Device->getUniversalQueueFamilyIndex();
        VkResult uniCmdPool = vkCreateCommandPool(m_Device->getVkDevice(), &poolInfo, nullptr, &m_UniversalCommandPool);
        C78E_CORE_SOFT_VALIDATE(uniCmdPool == VK_SUCCESS, "VulkanGraphicsContext::init: failed to create CommandPool!");

        // requires physical device - no access until Logical Device is created
        m_SurfaceCapabilities = fetchSurfaceCapabilities();
        m_SurfaceSupportedFormats = fetchSurfaceFormats();
        m_SurfaceSupportedPresentModes = fetchSurfacePresentModes();

    }

    void VulkanGraphicsContext::shutdown() {
        Ref<VulkanGraphicsInstance> vulkanInstance = GraphicsInstance::getAs<VulkanGraphicsInstance>();

        if(m_UniversalCommandPool) {
            vkDestroyCommandPool(m_Device->getVkDevice(), m_UniversalCommandPool, nullptr);
        }

        m_SwapChain = nullptr;

        if(m_VkSurface) {
            vkDestroySurfaceKHR(vulkanInstance->getInstance(), m_VkSurface, nullptr);
        }
    }

    const VkSurfaceCapabilitiesKHR& VulkanGraphicsContext::getSurfaceCapabilities() const {
        return m_SurfaceCapabilities;
    }
    const std::vector<VkSurfaceFormatKHR>& VulkanGraphicsContext::getSurfaceFormats() const {
        return m_SurfaceSupportedFormats;
    }
    const std::vector<VkPresentModeKHR>& VulkanGraphicsContext::getSurfacePresentModes() const {
        return m_SurfaceSupportedPresentModes;
    }

    VkSurfaceCapabilitiesKHR VulkanGraphicsContext::fetchSurfaceCapabilities() const {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Device->getVkPhysicalDevice(), m_VkSurface, &capabilities);
        return capabilities;
    }
    std::vector<VkSurfaceFormatKHR> VulkanGraphicsContext::fetchSurfaceFormats() const {
        std::vector<VkSurfaceFormatKHR> surfaceFormats;

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->getVkPhysicalDevice(), m_VkSurface, &formatCount, nullptr);

        if (formatCount != 0) {
            surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->getVkPhysicalDevice(), m_VkSurface, &formatCount, surfaceFormats.data());
        }
        return surfaceFormats;
    }
    std::vector<VkPresentModeKHR> VulkanGraphicsContext::fetchSurfacePresentModes() const {
        std::vector<VkPresentModeKHR> surfacePresentModes;

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device->getVkPhysicalDevice(), m_VkSurface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            surfacePresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device->getVkPhysicalDevice(), m_VkSurface, &presentModeCount, surfacePresentModes.data());
        }
        return surfacePresentModes;
    }

    uint32_t VulkanGraphicsContext::beginFrame() {
        C78E_CORE_VALIDATE(m_SwapChain, return false, "VulkanGraphicsContext::beginFrame: Called without SwapChain!");
        Ref<VulkanSwapChain> vulkanSwapChain = castRef<VulkanSwapChain>(m_SwapChain);
        C78E_CORE_VALIDATE(vulkanSwapChain, return false, "VulkanGraphicsContext::nextFrame: SwapChain is not of type VulkanSwapChain!");

        const uint32_t frameIndex = m_FrameIndex;
        m_FrameIndex = (m_FrameIndex + 1) % vulkanSwapChain->getFrameCount();

        Ref<FrameBuffer> frameBuffer = vulkanSwapChain->aquireNextFramebuffer(frameIndex);
        if (frameIndex >= m_InFlightFrameBuffers.size()) {
            m_InFlightFrameBuffers.resize(frameIndex + 1);
        }
        m_InFlightFrameBuffers[frameIndex] = frameBuffer;

        return frameIndex;
    }

    bool VulkanGraphicsContext::submit(uint32_t frameIndex, Ref<CommandBuffer> commandBuffer) {
        C78E_CORE_VALIDATE(m_SwapChain, return false, "VulkanGraphicsContext::submit: Called without SwapChain!");
		C78E_CORE_VALIDATE(commandBuffer, return false, "VulkanGraphicsContext::submit: CommandBuffer is null!");
        Ref<VulkanCommandBuffer> vulkanCommandBuffer = castRef<VulkanCommandBuffer>(commandBuffer);
		C78E_CORE_VALIDATE(vulkanCommandBuffer, return false, "VulkanGraphicsContext::submit: CommandBuffer is not of type VulkanCommandBuffer!");
		Ref<VulkanSwapChain> vulkanSwapChain = castRef<VulkanSwapChain>(m_SwapChain);
		C78E_CORE_VALIDATE(vulkanSwapChain, return false, "VulkanGraphicsContext::submit: SwapChain is not of type VulkanSwapChain!");

		const bool requiresGraphics = vulkanCommandBuffer->getRequiredVkQueueFlags() & VK_QUEUE_GRAPHICS_BIT;
		const bool requiresCompute = vulkanCommandBuffer->getRequiredVkQueueFlags() & VK_QUEUE_COMPUTE_BIT;
		const bool requiresTransfer = vulkanCommandBuffer->getRequiredVkQueueFlags() & VK_QUEUE_TRANSFER_BIT;

		//TODO: select queue based on command buffer requirements
		C78E_CORE_VALIDATE(requiresGraphics, return false, "VulkanGraphicsContext::submit: Currently only graphics command buffers are supported!");
        
		// Submit command buffer to a qualified queue
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.pWaitDstStageMask = waitStages;
        
		// Command buffer wait semaphores
        VkSemaphore waitSemaphores[] = { vulkanSwapChain->getImageAvailableSemaphore(frameIndex) };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;

		// Command buffer signal semaphores
        VkSemaphore signalSemaphores[] = { vulkanSwapChain->getFinishedRenderSemaphore(frameIndex) };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        
		//TODO: multiple command buffers submission - allow when more complex command buffer recording is needed
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = vulkanCommandBuffer->getVkCommandBufferPtr();

        insertCommandBuffer(frameIndex, commandBuffer);

        VkResult result = vkQueueSubmit(
            m_Device->getUniversalVkQueue(),
            1,
            &submitInfo,
            vulkanSwapChain->getInFlightFence(frameIndex)
		);
        
        return true;
    }

    bool VulkanGraphicsContext::submit(uint32_t frameIndex, const std::vector<Ref<CommandBuffer>>& commandBuffers) {
        return false;
    }

    bool VulkanGraphicsContext::endFrame(uint32_t frameIndex) {
        C78E_CORE_VALIDATE(m_SwapChain, return false, "VulkanGraphicsContext::endFrame: Called without SwapChain!");
        C78E_CORE_VALIDATE(frameIndex < m_InFlightFrameCommandBuffers.size(), return false, "frameIndex out of bounds!");
        Ref<VulkanSwapChain> vulkanSwapChain = castRef<VulkanSwapChain>(m_SwapChain);
        C78E_CORE_VALIDATE(vulkanSwapChain, return false, "VulkanGraphicsContext::endFrame: SwapChain is not of type VulkanSwapChain!");

        // Present if this command buffer is targeting the swap chain
        for (uint32_t i = m_InFlightFrameCommandBuffers[frameIndex].first; i < m_InFlightFrameCommandBuffers[frameIndex].second; i++) {
            Ref<VulkanCommandBuffer> vulkanCommandBuffer = castRef<VulkanCommandBuffer>(m_SubmittedCommandBuffers[i]);
            C78E_CORE_VALIDATE(vulkanCommandBuffer, return false, "VulkanGraphicsContext::submit: CommandBuffer is not of type VulkanCommandBuffer!");
            
            if ((vulkanCommandBuffer->getRequiredVkQueueFlags() & VK_QUEUE_GRAPHICS_BIT) && vulkanCommandBuffer->hasSwapChainTarget()) {
                VkPresentInfoKHR presentInfo = {};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

                // wait for all CommandBuffer signal semaphores -> rendering finished
                VkSemaphore waitSemaphores[] = { vulkanSwapChain->getFinishedRenderSemaphore(frameIndex) };
                presentInfo.waitSemaphoreCount = 1;
                presentInfo.pWaitSemaphores = waitSemaphores;

                VkSwapchainKHR swapChains[] = { vulkanSwapChain->getVkSwapChain() };
                presentInfo.swapchainCount = 1;
                presentInfo.pSwapchains = swapChains;

                const uint32_t imageIndex = m_InFlightFrameBuffers[frameIndex]->getSwapChainImageIndex();
                presentInfo.pImageIndices = &imageIndex;

                VkResult result = vkQueuePresentKHR(m_Device->getPresentVkQueue(), &presentInfo);
                //TODO: handle result - suboptimal, etc

                break; // One Present per Frame only
            }
        }
        
        VkResult waitIdleResult = vkDeviceWaitIdle(m_Device->getVkDevice());
		C78E_CORE_VALIDATE(waitIdleResult == VK_SUCCESS, return false, "VulkanGraphicsContext::endFrame: VkDevice - waitIdle failed!");

        return true;
    }

    void VulkanGraphicsContext::insertCommandBuffer(uint32_t frameIndex, Ref<CommandBuffer> commandBuffer) {

        // frameIndex superceeds the current registered in flight buffer count
        if (frameIndex >= m_InFlightFrameCommandBuffers.size()) {
            std::pair<uint32_t, uint32_t> base = { 0, 0 };
            if (!m_InFlightFrameCommandBuffers.empty()) {
                const uint32_t lastElem = m_InFlightFrameCommandBuffers.back().second;
                base = { lastElem, lastElem };
            }
            const uint32_t currentSize = m_InFlightFrameCommandBuffers.size();
            const uint32_t newSize = frameIndex + 1;

            m_InFlightFrameCommandBuffers.resize(newSize);
            for (uint32_t i = currentSize; i < newSize; i++) {
                m_InFlightFrameCommandBuffers[i] = base;
            }
        }

        // insert at frameIndex' last element
        const uint32_t insertIndex = m_InFlightFrameCommandBuffers[frameIndex].second;

        // std::vector::insert handles insert(end(), ..) as push_back alr, so insertIndex = size is fine
        C78E_CORE_ASSERT(insertIndex <= m_SubmittedCommandBuffers.size(), "insertIndex was out of bounds!");
        m_SubmittedCommandBuffers.insert(m_SubmittedCommandBuffers.begin() + insertIndex, commandBuffer);

        uint32_t followingIndex = m_InFlightFrameCommandBuffers[frameIndex].second++;

        // handle following elements
        for (uint32_t i = frameIndex + 1; i < m_InFlightFrameCommandBuffers.size(); i++) {
            m_InFlightFrameCommandBuffers[i].first = followingIndex;
            followingIndex = m_InFlightFrameCommandBuffers[i].second++;
        }
    }
}
