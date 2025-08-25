#pragma once
#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {
	
	class VulkanGraphicsInstance : public GraphicsInstance {
	public:
		VulkanGraphicsInstance();
		~VulkanGraphicsInstance();

		VkInstance getInstance() { return m_VkInstance; }
		const std::vector<VkExtensionProperties>& getAvailableExtensions() { return m_AvailableExtensions; }
		const std::vector<VkLayerProperties>& getAvailableLayers() { return m_AvailableLayers; }

		Ref<VulkanDevice> pickDevice(VkSurfaceKHR surface);

		API getAPIType() const override { return API::Vulkan; }

	private:
		void init();
		void shutdown();

		void fetchAvailableExtensions();
		void fetchAvailableLayers();
		bool checkExtensionSupport(const char* extensionName);
		bool checkExtensionSupport(const std::vector<const char*>& extensionNames);
		bool checkLayerSupport(const char* layerName);
		bool checkLayerSupport(const std::vector<const char*>& layerNames);
		
	private:
		std::vector<VkExtensionProperties> m_AvailableExtensions;
		std::vector<VkLayerProperties> m_AvailableLayers;
		VkInstance m_VkInstance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		
	};

}
