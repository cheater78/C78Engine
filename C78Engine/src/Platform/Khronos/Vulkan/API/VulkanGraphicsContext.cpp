#include "C78EPCH.h"
#include "VulkanGraphicsContext.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsInstance.h>

#include <C78E/Core/Application/Application.h>
//#include <Platform/Khronos/Vulkan/API/Command/VulkanCommandBuffer.h>

namespace C78E {

    VulkanGraphicsContext::VulkanGraphicsContext(Window& window)
		: GraphicsContext(window) {
		init();
	}
	VulkanGraphicsContext::~VulkanGraphicsContext() {
        shutdown();
	}

    void VulkanGraphicsContext::init() {
		Ref<VulkanGraphicsInstance> vulkanInstance = GraphicsInstance::getAs<VulkanGraphicsInstance>();

        // Create window surface
        VkResult result = glfwCreateWindowSurface(vulkanInstance->getInstance(), m_Window.getNativeWindowAs<GLFWwindow>(), nullptr, &m_VkSurface);
        C78E_CORE_ASSERT(result == VK_SUCCESS, "VulkanGraphicsContext::init: Failed to create window surface!");

		// pick a suitable device
		m_Device = vulkanInstance->pickDevice(m_VkSurface);

        // Create SwapChain
        //m_SwapChain = VulkanSwapChain::create( m_Device, swapChainConfig, m_VkSurface);

		// Create Command Pool

    }

    void VulkanGraphicsContext::shutdown() {
        Ref<VulkanGraphicsInstance> vulkanInstance = GraphicsInstance::getAs<VulkanGraphicsInstance>();
        if(m_VkSurface) {
            vkDestroySurfaceKHR(vulkanInstance->getInstance(), m_VkSurface, nullptr);
        }
    }
    
    void VulkanGraphicsContext::aquireNextSwapChainImage() {

        //TODO: actually acquire next Swap image

        m_Device->waitIdle();
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
