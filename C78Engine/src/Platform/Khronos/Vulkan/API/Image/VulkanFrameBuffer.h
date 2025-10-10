#pragma once
#include <C78E/Graphics/API/Image/FrameBuffer.h>

#include <Platform/Khronos/Vulkan/Core/VulkanConversions.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	class VulkanSwapChain;

	struct VulkanFrameBufferAttachmentResources {
		VkImage image = VK_NULL_HANDLE; // Vulkan Image Handle
		VkImageView imageView = VK_NULL_HANDLE; // Vulkan Image View Handle
		bool isSwapChainTarget = false; // Is this attachment a swap chain target?
		VkDeviceMemory memory = VK_NULL_HANDLE; // Vulkan Device Memory Handle

		VkSampler sampler = VK_NULL_HANDLE; // optional Vulkan Sampler Handle
	};

	class VulkanFrameBuffer : public FrameBuffer {
	public:
		VulkanFrameBuffer(GraphicsContext& ctx, const FrameBufferConfig& cfg, Ref<RenderPass> renderPass,
			VulkanSwapChain* vulkanSwapChain, uint32_t swapChainAttachmentIndex, VkImage swapChainImage, uint32_t swapChainImageIndex);
		VulkanFrameBuffer(GraphicsContext& ctx, const FrameBufferConfig& cfg, Ref<RenderPass> renderPass);
		virtual ~VulkanFrameBuffer();

		virtual void resize(ImageSize size) override;
		virtual bool isSwapChainTarget() const override;
		virtual uint32_t getSwapChainImageIndex() const override;

	public:
		VkFramebuffer getVkFrameBuffer() const { return m_VkFrameBuffer; }
		VkImage getColorAttachmentVkImage(uint32_t index) const;


		void setImageAvailableSemaphore(VkSemaphore semaphore) { m_ImageAvailableSemaphore = semaphore; }
		void setRenderFinishedSemaphore(VkSemaphore semaphore) { m_RenderFinishedSemaphore = semaphore; }
		VkSemaphore getImageAvailableSemaphore() const { return m_ImageAvailableSemaphore; }
		VkSemaphore getRenderFinishedSemaphore() const { return m_RenderFinishedSemaphore; }
	private:
		bool createVulkanFrameBuffer(VulkanSwapChain* vulkanSwapChain = nullptr, uint32_t swapChainAttachmentIndex = -1, VkImage swapChainImage = VK_NULL_HANDLE);
		void destroyVulkanFrameBuffer();

		bool createVulkanFrameBufferAttachment(VulkanFrameBufferAttachmentResources& attachment, const FrameBufferAttachmentSpecification& spec, ImageSize size, uint32_t samples, VkImage swapChainImage = VK_NULL_HANDLE);
		void destroyVulkanFrameBufferAttachment(VulkanFrameBufferAttachmentResources& attachment);

	private:
		Ref<VulkanDevice> m_Device; // Vulkan Device Reference
		VkFramebuffer m_VkFrameBuffer = VK_NULL_HANDLE; // FrameBuffer Handle
		std::vector<VulkanFrameBufferAttachmentResources> m_VulkanAttachments; // Vulkan Attachment Objects

		// opt. for swap chain target
		VulkanSwapChain* m_VulkanSwapChain = nullptr;
		uint32_t m_SwapChainImageIndex = -1;

		// sync
		VkSemaphore m_ImageAvailableSemaphore = VK_NULL_HANDLE; //(non owning) signaled when the image is unused, e.g. after vkAcquireNextImageKHR, after presentation, or done being read from in general
		VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE; //(non owning) signaled when rendering commands are finished executing, indicates that the image is ready for presentation or reading
	};

}
