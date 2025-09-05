#include "C78EPCH.h"
#include "FrameBuffer.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <C78E/Graphics/API/GraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanFrameBuffer.h>

namespace C78E {

	Ref<FrameBuffer> FrameBuffer::create(GraphicsContext& ctx, const FrameBufferSpecification& spec, Ref<RenderPass> renderPass) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:  return createRef<VulkanFrameBuffer>(ctx, spec, renderPass);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		}
		return nullptr;
	}

	FrameBuffer::FrameBuffer(GraphicsContext& ctx, const FrameBufferSpecification& spec, Ref<RenderPass> renderPass)
		: m_GraphicsContext(ctx), m_Specification(spec), m_RenderPass(renderPass) {
	}

	const FrameBufferSpecification& FrameBuffer::getFrameBufferSpecification() const {
		return m_Specification;
	}

	const std::vector<FrameBufferAttachmentSpecification>& FrameBuffer::getColorAttachmentSpecifications() const {
		return getFrameBufferSpecification().colorAttachmentSpecifications;
	}

	bool FrameBuffer::hasDepthAttachment() const {
		return getFrameBufferSpecification().hasDepthAttachment;
	}

	const FrameBufferAttachmentSpecification& FrameBuffer::getDepthAttachmentSpecifications() const {
		return getFrameBufferSpecification().depthAttachmentSpecification;
	}

	bool FrameBuffer::isMultisampled() const {
		return m_Specification.samples;
	}

	uint32_t FrameBuffer::getSamples() const {
		return m_Specification.samples;
	}

}

