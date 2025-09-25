#include "C78EPCH.h"
#include "FrameBuffer.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <C78E/Graphics/API/GraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanFrameBuffer.h>

namespace C78E {

	Ref<FrameBuffer> FrameBuffer::create(GraphicsContext& ctx, const FrameBufferConfig& cfg, Ref<RenderPass> renderPass) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:  return createRef<VulkanFrameBuffer>(ctx, cfg, renderPass);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		}
		return nullptr;
	}

	FrameBuffer::FrameBuffer(GraphicsContext& ctx, const FrameBufferConfig& cfg, Ref<RenderPass> renderPass)
		: m_GraphicsContext(ctx), m_Config(cfg), m_RenderPass(renderPass) {
	}

	const FrameBufferConfig& FrameBuffer::getFrameBufferConfig() const {
		return m_Config;
	}

	const ImageSize FrameBuffer::getSize() const {
		return m_Config.size;
	}

	const std::vector<FrameBufferAttachmentSpecification>& FrameBuffer::getAttachmentSpecifications() const {
		return m_Config.attachmentSpecifications;
	}

	bool FrameBuffer::hasDepthAttachment() const {
		return m_Config.activeDepthAttachment != std::numeric_limits<uint32_t>::max();
	}

	const FrameBufferAttachmentSpecification& FrameBuffer::getDepthAttachmentSpecification() const {
		const uint32_t& depthIndex = m_Config.activeDepthAttachment;
		C78E_CORE_ASSERT(depthIndex != std::numeric_limits<uint32_t>::max(), "FrameBuffer::getDepthAttachmentSpecification: no active depth attachment set!");
		const std::vector<FrameBufferAttachmentSpecification> attachments = m_Config.attachmentSpecifications;
		C78E_CORE_ASSERT(depthIndex < attachments.size(), "FrameBuffer::getDepthAttachmentSpecification: depth attachment index was out of bounds!(index: {}, attachments: {})", depthIndex, attachments.size());
		return attachments[depthIndex];
	}

	bool FrameBuffer::isMultisampled() const {
		return m_Config.multiSample != MultiSample::X1;
	}

	uint32_t FrameBuffer::getSamples() const {
		return m_Config.multiSample;
	}

}

