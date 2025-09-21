#pragma once
#include <C78E/Graphics/API/GraphicsContext.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	class VulkanCommandBuffer;

	class VulkanGraphicsContext : public GraphicsContext {
	private:
		static std::vector<const char*> getRequiredExtensionNames();
		static std::vector<const char*> getRequiredLayerNames();
	public:
		VulkanGraphicsContext(Window& window);
		virtual ~VulkanGraphicsContext();

		virtual Ref<CommandBuffer> createCommandBuffer();

		virtual uint32_t beginFrame() override;
		virtual bool submit(uint32_t frameIndex, Ref<CommandBuffer> commandBuffer) override;
		virtual bool submit(uint32_t frameIndex, const std::vector<Ref<CommandBuffer>>& commandBuffers) override;
		virtual bool endFrame(uint32_t frameIndex) override;
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

		std::vector<std::pair<uint32_t, uint32_t>> m_InFlightFrameCommandBuffers;
		std::vector<Ref<CommandBuffer>> m_SubmittedCommandBuffers;
		std::vector<Ref<FrameBuffer>> m_InFlightFrameBuffers; //TODO: used, but needed?

		void insertCommandBuffer(uint32_t frameIndex, Ref<CommandBuffer> commandBuffer);

		uint32_t m_FrameIndex = 0;

	};

}
