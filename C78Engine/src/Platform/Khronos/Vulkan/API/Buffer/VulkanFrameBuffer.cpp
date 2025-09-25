#include "C78EPCH.h"
#include "VulkanFrameBuffer.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/Core/Buffer/VulkanBuffer.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanRenderPass.h>

#include <Platform/Khronos/Vulkan/API/SwapChain/VulkanSwapChain.h>
#include <Platform/Khronos/Vulkan/API/Texture/VulkanTexture.h>

namespace C78E {

	VulkanFrameBuffer::VulkanFrameBuffer(GraphicsContext& ctx, const FrameBufferConfig& spec, Ref<RenderPass> renderPass, VulkanSwapChain* vulkanSwapChain, uint32_t swapChainAttachmentIndex, VkImage swapChainImage, uint32_t swapChainImageIndex)
		: FrameBuffer(ctx, spec, renderPass), m_SwapChainImageIndex(swapChainImageIndex) {
		C78E_CORE_ASSERT(createVulkanFrameBuffer(vulkanSwapChain, swapChainAttachmentIndex, swapChainImage), "VulkanFrameBuffer::VulkanFrameBuffer: Creation as OffScreenTarget failed!");
	}

	VulkanFrameBuffer::VulkanFrameBuffer(GraphicsContext& ctx, const FrameBufferConfig& spec, Ref<RenderPass> renderPass)
		: FrameBuffer(ctx, spec, renderPass) {
		C78E_CORE_ASSERT(createVulkanFrameBuffer(), "VulkanFrameBuffer::VulkanFrameBuffer: Creation as OffScreenTarget failed!");
	}

	VulkanFrameBuffer::~VulkanFrameBuffer() {
		destroyVulkanFrameBuffer();
    }

	void VulkanFrameBuffer::resize(ImageSize size) {
	}

	bool VulkanFrameBuffer::isSwapChainTarget() const {
		return m_VulkanSwapChain;
	}

	uint32_t VulkanFrameBuffer::getSwapChainImageIndex() const {
		return m_SwapChainImageIndex;
	}


	VkImage VulkanFrameBuffer::getColorAttachmentVkImage(uint32_t index) const {
		return m_VulkanAttachments[index].image;
	}

	bool VulkanFrameBuffer::createVulkanFrameBuffer(VulkanSwapChain* vulkanSwapChain, uint32_t swapChainAttachmentIndex, VkImage swapChainImage) {
		VulkanGraphicsContext& ctx = m_GraphicsContext.getAs<VulkanGraphicsContext>();
		m_Device = ctx.getDevice();
		Ref<VulkanRenderPass> vulkanRenderPass = castRef<VulkanRenderPass>(m_RenderPass);
		C78E_CORE_VALIDATE(vulkanRenderPass, return false, "VulkanFrameBuffer::createVulkanFrameBuffer: RenderPass is not a VulkanRenderPass!");
		m_VulkanSwapChain = vulkanSwapChain;
		
		const bool recreate = m_VkFrameBuffer != VK_NULL_HANDLE;
		const bool isSwapChainTarget = !vulkanSwapChain || !swapChainImage || (swapChainAttachmentIndex != -1);

		if(recreate) {
			destroyVulkanFrameBuffer();
		}

		

		const ImageSize& size = m_Config.size;
		const std::vector<FrameBufferAttachmentSpecification>& attachmentSpecs = m_Config.attachmentSpecifications;
		const size_t attachmentCount = attachmentSpecs.size();
		const uint32_t samples = m_Config.multiSample;

		m_VulkanAttachments.clear();
		m_VulkanAttachments.resize(attachmentCount);

		for(size_t i = 0; i < attachmentSpecs.size(); i++) {
			if(!createVulkanFrameBufferAttachment(
				m_VulkanAttachments[i],
				attachmentSpecs[i],
				size,
				samples,
				(isSwapChainTarget && i == swapChainAttachmentIndex) ? swapChainImage : VK_NULL_HANDLE)) {
				return false;
			}
		}

		std::vector<VkImageView> attachmentImageViews(attachmentCount);
		for(size_t i = 0; i < attachmentCount; i++) {
			attachmentImageViews[i] = m_VulkanAttachments[i].imageView;
		}


		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = vulkanRenderPass->getVkRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachmentCount);
		framebufferInfo.pAttachments = attachmentImageViews.data();
		framebufferInfo.width = size.x;
		framebufferInfo.height = size.y;
		framebufferInfo.layers = 1;

		VkResult framebufferCreateResult = vkCreateFramebuffer(
			m_Device->getVkDevice(),
			&framebufferInfo,
			nullptr,
			&m_VkFrameBuffer);
		C78E_CORE_VALIDATE(framebufferCreateResult == VK_SUCCESS, return false, "VulkanFrameBuffer::createVulkanFrameBuffer: Failed to create VkFrameBuffer!");

		return true;
	}

	void VulkanFrameBuffer::destroyVulkanFrameBuffer() {
		// Destroy all attachments
		for(VulkanFrameBufferAttachmentResources& resource : m_VulkanAttachments) {
			destroyVulkanFrameBufferAttachment(resource);
		}
		// Destroy the framebuffer
		if(m_VkFrameBuffer != VK_NULL_HANDLE) {
			vkDestroyFramebuffer(m_Device->getVkDevice(), m_VkFrameBuffer, nullptr);
		}
	}

	bool VulkanFrameBuffer::createVulkanFrameBufferAttachment(VulkanFrameBufferAttachmentResources& attachment, const FrameBufferAttachmentSpecification& spec, ImageSize size, uint32_t samples, VkImage swapChainImage) {
		const bool isSwapChainTarget = swapChainImage;
		const VkFormat vkFormat = toVkFormat(spec.format);

		if (!isSwapChainTarget) {
			attachment.isSwapChainTarget = false;
			// Create Image
			VkImageCreateInfo imageInfo = {};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.format = vkFormat;
			imageInfo.extent.width = size.x;
			imageInfo.extent.height = size.y;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.samples = static_cast<VkSampleCountFlagBits>(samples);
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.usage = 
				VK_IMAGE_USAGE_SAMPLED_BIT |
				(ImageFormat::isColorFormat(spec.format) ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0) |
				(ImageFormat::isDepthFormat(spec.format) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : 0);

			VkResult result = vkCreateImage(m_Device->getVkDevice(), &imageInfo, nullptr, &attachment.image);
			C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanFrameBuffer::VulkanFrameBuffer: Failed to create image!");

			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(m_Device->getVkDevice(), attachment.image, &memRequirements);
			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(m_Device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			result = vkAllocateMemory(m_Device->getVkDevice(), &allocInfo, nullptr, &attachment.memory);
			C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanFrameBuffer::VulkanFrameBuffer: Failed to allocate memory for image!");
			vkBindImageMemory(m_Device->getVkDevice(), attachment.image, attachment.memory, 0);

		} else {
			attachment.image = swapChainImage;
			attachment.isSwapChainTarget = true;
		}

		// Create Image View
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = attachment.image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = vkFormat;
		viewInfo.subresourceRange.aspectMask = 
			((ImageFormat::isColorFormat(spec.format)) ? VK_IMAGE_ASPECT_COLOR_BIT : 0) |
			((ImageFormat::isDepthFormat(spec.format)) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0);
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		VkResult result = vkCreateImageView(m_Device->getVkDevice(), &viewInfo, nullptr, &attachment.imageView);
		C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanFrameBuffer::VulkanFrameBuffer: Failed to create image view!");

		// Create Sampler
		if(spec.format != ImageFormat::None) {
			VkSamplerCreateInfo samplerInfo = {};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = toVkFilter(spec.magFilter);
			samplerInfo.minFilter = toVkFilter(spec.minFilter);
			samplerInfo.addressModeU = toVkSamplerAddressMode(spec.wrapU);
			samplerInfo.addressModeV = toVkSamplerAddressMode(spec.wrapV);
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT; // 3D framebuffers?
			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = static_cast<float>(samples);
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			result = vkCreateSampler(m_Device->getVkDevice(), &samplerInfo, nullptr, &attachment.sampler);
			C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanFrameBuffer::VulkanFrameBuffer: Failed to create sampler!");
		}

		return true;
	}

	void VulkanFrameBuffer::destroyVulkanFrameBufferAttachment(VulkanFrameBufferAttachmentResources& attachment) {
		if(attachment.sampler != VK_NULL_HANDLE) {
			vkDestroySampler(m_Device->getVkDevice(), attachment.sampler, nullptr);
		}
		if(attachment.imageView != VK_NULL_HANDLE) {
			vkDestroyImageView(m_Device->getVkDevice(), attachment.imageView, nullptr);
		}
		if(attachment.image != VK_NULL_HANDLE && !attachment.isSwapChainTarget) {
			vkDestroyImage(m_Device->getVkDevice(), attachment.image, nullptr);
		}
		if(attachment.memory != VK_NULL_HANDLE && !attachment.isSwapChainTarget) {
			vkFreeMemory(m_Device->getVkDevice(), attachment.memory, nullptr);
		}
	}

}
