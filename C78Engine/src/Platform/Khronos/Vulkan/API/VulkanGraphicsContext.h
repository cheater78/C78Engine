#pragma once
#include <C78E/Graphics/API/GraphicsContext.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	class VulkanGraphicsContext : public GraphicsContext {
	private:
		static std::vector<const char*> getRequiredExtensionNames();
		static std::vector<const char*> getRequiredLayerNames();
	public:
		VulkanGraphicsContext(Window& window);
		virtual ~VulkanGraphicsContext();

		
		//virtual bool submit(Ref<CommandBuffer> commandBuffer) override;
		virtual void aquireNextSwapChainImage() override;
	public:
		Ref<VulkanDevice> getDevice() const { return m_Device; }
		VkSurfaceKHR getSurface() const { return m_VkSurface; }
	private:
		void init();
		void shutdown();
	private:
		Ref<VulkanDevice> m_Device;
		VkSurfaceKHR m_VkSurface = VK_NULL_HANDLE;
		VkSurfaceCapabilitiesKHR m_SurfaceCapabilities;
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
	};

}
