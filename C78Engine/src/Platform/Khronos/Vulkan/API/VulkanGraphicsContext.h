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

		virtual Ref<CommandBuffer> createCommandBuffer();
		//virtual bool submit(Ref<CommandBuffer> commandBuffer) = 0;
		//
		//virtual Ref<VertexBuffer> createVertexBuffer() = 0;
		//virtual Ref<IndexBuffer> createIndexBuffer() = 0;
		//virtual Ref<UniformBuffer> createUniformBuffer() = 0;
		//virtual Ref<StorageBuffer> createStorageBuffer() = 0;
		
		//virtual bool submit(Ref<CommandBuffer> commandBuffer) override;
	public:
		Ref<VulkanDevice> getDevice() const { return m_Device; }
		VkSurfaceKHR getSurface() const { return m_VkSurface; }

		const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities() const;
		const std::vector<VkSurfaceFormatKHR>& getSurfaceFormats() const;
		const std::vector<VkPresentModeKHR>& getSurfacePresentModes() const;
	private:
		void init();
		void shutdown();

		VkSurfaceCapabilitiesKHR fetchSurfaceCapabilities() const;
		std::vector<VkSurfaceFormatKHR> fetchSurfaceFormats() const;
		std::vector<VkPresentModeKHR> fetchSurfacePresentModes() const;
	private:
		Ref<VulkanDevice> m_Device;
		VkSurfaceKHR m_VkSurface = VK_NULL_HANDLE;
		VkSurfaceCapabilitiesKHR m_SurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> m_SurfaceSupportedFormats;
		std::vector<VkPresentModeKHR> m_SurfaceSupportedPresentModes;

		VkCommandPool m_UniversalCommandPool = VK_NULL_HANDLE;
	};

}
