#include "C78EPCH.h"
#include "vulkan/vulkan_core.h"
#include "VulkanGraphicsContext.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/SwapChain/VulkanSwapChain.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanCommandBuffer.h>

namespace C78E {

    GraphicsContextLimits toGraphicsContextLimits(VkPhysicalDeviceLimits limits) {
        GraphicsContextLimits gcl;
        // Vertex
        gcl.maxVertexAttributeCount = limits.maxVertexInputAttributes;
        gcl.maxVertexBindings = limits.maxVertexInputBindings;
        // Uniform
        gcl.maxPushConstantSize = limits.maxPushConstantsSize;
        gcl.maxUniformBuffers = limits.maxDescriptorSetUniformBuffers;
        gcl.maxPerStageUniformBuffers = limits.maxPerStageDescriptorUniformBuffers;

        return gcl;
    }

    VulkanGraphicsContext::VulkanGraphicsContext(Window& window)
		: GraphicsContext(window) {
		init();
	}
	VulkanGraphicsContext::~VulkanGraphicsContext() {
        shutdown();
	}

    Ref<CommandBuffer> VulkanGraphicsContext::createCommandBuffer() {
		return createRef<VulkanCommandBuffer>(*this, true, CommandBuffer::Usage::Auto, getVkCommandPoolFor(CommandBuffer::Usage::Auto));
    }

    Scope<CommandBuffer> VulkanGraphicsContext::beginSingleTimeCommand(CommandBuffer::UsageFlags usage) {
        Scope<CommandBuffer> cmdBuf = createScope<VulkanCommandBuffer>(*this, false, usage, getVkCommandPoolFor(usage));
        //TODO: how todo submissions(blocking / non blocking) on destruction? - separate class? / Scope?
        // move Vk objects to some storage in ctx -> execute async for non blocking
        return std::move(cmdBuf);
    }

    VkSurfaceCapabilitiesKHR VulkanGraphicsContext::getSurfaceCapabilities() const {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Device->getVkPhysicalDevice(), m_VkSurface, &capabilities);
        return capabilities;
    }
    std::vector<VkSurfaceFormatKHR> VulkanGraphicsContext::getSurfaceFormats() const {
        std::vector<VkSurfaceFormatKHR> surfaceFormats;

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->getVkPhysicalDevice(), m_VkSurface, &formatCount, nullptr);

        if (formatCount != 0) {
            surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->getVkPhysicalDevice(), m_VkSurface, &formatCount, surfaceFormats.data());
        }
        return surfaceFormats;
    }
    std::vector<VkPresentModeKHR> VulkanGraphicsContext::getSurfacePresentModes() const {
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

        const uint32_t frameIndex = m_FrameIndex; // No ASYNC!
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

        m_SubmittedCommandBuffers.push_back(commandBuffer);

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

    bool VulkanGraphicsContext::submit(Ref<CommandBuffer> commandBuffer) {
        Ref<VulkanCommandBuffer> vulkanCommandBuffer = castRef<VulkanCommandBuffer>(commandBuffer);
        C78E_CORE_VALIDATE(vulkanCommandBuffer, return false, "VulkanGraphicsContext::submit: CommandBuffer is not of type VulkanCommandBuffer!");
        const bool requiresGraphics = vulkanCommandBuffer->getRequiredVkQueueFlags() & VK_QUEUE_GRAPHICS_BIT;
        const bool requiresCompute = vulkanCommandBuffer->getRequiredVkQueueFlags() & VK_QUEUE_COMPUTE_BIT;
        const bool requiresTransfer = vulkanCommandBuffer->getRequiredVkQueueFlags() & VK_QUEUE_TRANSFER_BIT;

        const bool isTransferOnly = !requiresGraphics && !requiresCompute && requiresTransfer;

        if(isTransferOnly) {
            return submitTransferOnlyCommandBuffer(commandBuffer);
		}
        return false;
    }

    bool VulkanGraphicsContext::endFrame(uint32_t frameIndex) {
        C78E_CORE_VALIDATE(m_SwapChain, return false, "VulkanGraphicsContext::endFrame: Called without SwapChain!");
        Ref<VulkanSwapChain> vulkanSwapChain = castRef<VulkanSwapChain>(m_SwapChain);
        C78E_CORE_VALIDATE(vulkanSwapChain, return false, "VulkanGraphicsContext::endFrame: SwapChain is not of type VulkanSwapChain!");

        // Present if this command buffer is targeting the swap chain
        for (auto& cB : m_SubmittedCommandBuffers) {
            Ref<VulkanCommandBuffer> vulkanCommandBuffer = castRef<VulkanCommandBuffer>(cB);
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
                C78E_CORE_VALIDATE(
                    result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR,
                    return false,
                    "VulkanGraphicsContext::endFrame: vkQueuePresentKHR failed!"
                );

                break; // One Present per Frame only
            }
        }

        const bool waitSucc = m_Device->waitIdle();

        m_SubmittedCommandBuffers.clear();
        return waitSucc;
    }

    const GraphicsContextLimits& GraphicsContext::getGraphicsContextLimits() const {
        return m_Limits;
    }

    bool VulkanGraphicsContext::copyBuffer(GPUBuffer& srcGPUBuffer, GPUBuffer& dstGPUBuffer, size_t size, size_t srcOffset, size_t dstOffset) {
        Ref<CommandBuffer> cmd = createCommandBuffer(); // TODO: Currently universal queue fam
        cmd->beginRecording();
        cmd->copyBuffer(srcGPUBuffer, dstGPUBuffer, size, srcOffset, dstOffset);
        cmd->endRecording();

        //TODO: general submission for all cmb buffs -> commit to correct queue auto
        Ref<VulkanCommandBuffer> vulkanCommandBuffer = castRef<VulkanCommandBuffer>(cmd);
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_TRANSFER_BIT;
        submitInfo.pWaitDstStageMask = &waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = vulkanCommandBuffer->getVkCommandBufferPtr();

        VkResult result = vkQueueSubmit(
            m_Device->getUniversalVkQueue(),
            1,
            &submitInfo,
            VK_NULL_HANDLE // wait fence
        );
        m_Device->waitUniversalQueueIdle(); // wait for completion, TODO: use fence
        return true; // TODO: ...
    }

    void VulkanGraphicsContext::init() {
        Ref<VulkanGraphicsInstance> vulkanInstance = GraphicsInstance::getAs<VulkanGraphicsInstance>();

        // Create window surface
        VkResult result = glfwCreateWindowSurface(vulkanInstance->getInstance(), m_Window.getNativeWindowAs<GLFWwindow>(), nullptr, &m_VkSurface);
        C78E_CORE_ASSERT(result == VK_SUCCESS, "VulkanGraphicsContext::init: Failed to create window surface!");

        // pick a suitable device
        m_Device = vulkanInstance->pickDevice(m_VkSurface);
        m_Limits = toGraphicsContextLimits(m_Device->getPhysicalDeviceProperties().limits);

    }

    void VulkanGraphicsContext::shutdown() {
        Ref<VulkanGraphicsInstance> vulkanInstance = GraphicsInstance::getAs<VulkanGraphicsInstance>();

        for (auto& cmd : m_SubmittedCommandBuffers) {
            cmd.reset(); // kill all buffers before the pool
        }

        m_SwapChain = nullptr;

        if (m_VkSurface) {
            vkDestroySurfaceKHR(vulkanInstance->getInstance(), m_VkSurface, nullptr);
        }
    }

    VkCommandPool VulkanGraphicsContext::getVkCommandPoolFor(CommandBuffer::UsageFlags usage) const {
        using Usage = CommandBuffer::Usage;
        if (!usage || usage & Usage::Graphics) { // Auto or Graphics
            return m_Device->getUniversalVkCommandPool();
        } else if (usage & Usage::Compute) {
            return m_Device->getComputeVkCommandPool();
        } else if (usage & Usage::Control) {
            return m_Device->getTransferVkCommandPool();
        }
        return VK_NULL_HANDLE;
    }

    bool VulkanGraphicsContext::submitComputeOnlyCommandBuffer(Ref<CommandBuffer> commandBuffer) {
        Ref<VulkanCommandBuffer> vulkanCommandBuffer = castRef<VulkanCommandBuffer>(commandBuffer);
        C78E_CORE_VALIDATE(vulkanCommandBuffer, return false, "VulkanGraphicsContext::submitTransferOnlyCommandBuffer: CommandBuffer is not of type VulkanCommandBuffer!");
        
        return false;
    }

    bool VulkanGraphicsContext::submitTransferOnlyCommandBuffer(Ref<CommandBuffer> commandBuffer) {
        Ref<VulkanCommandBuffer> vulkanCommandBuffer = castRef<VulkanCommandBuffer>(commandBuffer);
        C78E_CORE_VALIDATE(vulkanCommandBuffer, return false, "VulkanGraphicsContext::submitTransferOnlyCommandBuffer: CommandBuffer is not of type VulkanCommandBuffer!");
        
        // Submit command buffer to a qualified queue
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_TRANSFER_BIT;
        submitInfo.pWaitDstStageMask = &waitStages;

        //// Command buffer wait semaphores
        //VkSemaphore waitSemaphores[] = { };
        //submitInfo.waitSemaphoreCount = 1;
        //submitInfo.pWaitSemaphores = waitSemaphores;
        //
        //// Command buffer signal semaphores
        //VkSemaphore signalSemaphores[] = { };
        //submitInfo.signalSemaphoreCount = 1;
        //submitInfo.pSignalSemaphores = signalSemaphores;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = vulkanCommandBuffer->getVkCommandBufferPtr();

        VkResult result = vkQueueSubmit(
            m_Device->getTransferVkQueue(),
            1,
            &submitInfo,
            VK_NULL_HANDLE // wait fence
        );
        return false;
    }

}
