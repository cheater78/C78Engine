#include "C78EPCH.h"
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
        C78E_CORE_SOFT_VALIDATE(uniCmdPool, "VulkanGraphicsContext::init: failed to create CommandPool!");

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
    /*
    bool VulkanGraphicsContext::submit(Ref<CommandBuffer> commandBuffer) {
		C78E_CORE_VALIDATE(commandBuffer, return false, "VulkanGraphicsContext::submit: CommandBuffer is null!");
        Ref<VulkanCommandBuffer> vulkanCommandBuffer = castRef<VulkanCommandBuffer>(commandBuffer);
		C78E_CORE_VALIDATE(vulkanCommandBuffer, return false, "VulkanGraphicsContext::submit: CommandBuffer is not of type VulkanCommandBuffer!");
        return m_Device->submitCommandBuffer(vulkanCommandBuffer);
    }
    */
    
}
