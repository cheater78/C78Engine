#pragma once
#include <C78E/Graphics/API/Buffer/FrameBuffer.h>

#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>
#include <Platform/Khronos/Vulkan/API/Texture/VulkanImageFormat.h>

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
		VulkanFrameBuffer(GraphicsContext& ctx, const FrameBufferSpecification& spec, Ref<RenderPass> renderPass, VulkanSwapChain* vulkanSwapChain, uint32_t swapChainAttachmentIndex, VkImage swapChainImage);
		VulkanFrameBuffer(GraphicsContext& ctx, const FrameBufferSpecification& spec, Ref<RenderPass> renderPass);
		virtual ~VulkanFrameBuffer();

		virtual void resize(ImageSize size) override;
		virtual bool isSwapChainTarget() const override;
	public:
		VkFramebuffer getVkFrameBuffer() const { return m_VkFrameBuffer; }
	private:
		bool createVulkanFrameBuffer(VulkanSwapChain* vulkanSwapChain = nullptr, uint32_t swapChainAttachmentIndex = -1, VkImage swapChainImage = VK_NULL_HANDLE);
		void destroyVulkanFrameBuffer();

		bool createVulkanFrameBufferAttachment(VulkanFrameBufferAttachmentResources& attachment, const FrameBufferAttachmentSpecification& spec, ImageSize size, uint32_t samples, VkImage swapChainImage = VK_NULL_HANDLE);
		void destroyVulkanFrameBufferAttachment(VulkanFrameBufferAttachmentResources& attachment);

	protected:
		// GraphicsContext& m_GraphicsContext;
		// FrameBufferSpecification m_Specification;
	private:
		Ref<VulkanDevice> m_Device; // Vulkan Device Reference
		VkFramebuffer m_VkFrameBuffer = VK_NULL_HANDLE; // FrameBuffer Handle
		VulkanFrameBufferAttachmentResources m_VulkanDepthAttachment;
		std::vector<VulkanFrameBufferAttachmentResources> m_VulkanColorAttachments; // Vulkan Attachment Objects

		VulkanSwapChain* m_VulkanSwapChain = nullptr;
	};

}
