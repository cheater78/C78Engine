#include "C78EPCH.h"
#include "VertexBuffer.h"

#include <C78E/Graphics/API/GraphicsContext.h>
#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanVertexBuffer.h>

namespace C78E {

	Ref<VertexBuffer> VertexBuffer::create(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, size_t vertexCount) {
		switch(GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanVertexBuffer>(ctx, layout, vertexCount);
		default:
			C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		}
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, Ref<StagingBuffer> stagingBuffer) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanVertexBuffer>(ctx, layout, stagingBuffer);
		default:
			C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		}
		return nullptr;
	}

	VertexBuffer::VertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, size_t vertexCount)
		: m_GraphicsContext(ctx), m_VertexInputRate(inputRate), m_Layout(layout), m_VertexCount(vertexCount) {
	}

	VertexInputRate VertexBuffer::getVertexInputRate() const {
		return m_VertexInputRate;
	}

	VertexBufferLayout VertexBuffer::getVertexBufferLayout() const {
		return m_VertexInputRate;
	}

	size_t VertexBuffer::getVertexCount() const {
		return m_VertexCount;
	}


}


