#include "C78EPCH.h"
#include "IndexBuffer.h"

#include <C78E/Graphics/API/GraphicsContext.h>
#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanIndexBuffer.h>

namespace C78E {

	Ref<IndexBuffer> IndexBuffer::create(GraphicsContext& ctx, const IndexLayout& layout, size_t indexCount) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanIndexBuffer>(ctx, layout, indexCount);
		default:
			C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		}
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(GraphicsContext& ctx, const IndexLayout& layout, Ref<StagingBuffer> stagingBuffer) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanIndexBuffer>(ctx, layout, stagingBuffer);
		default:
			C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		}
		return nullptr;
	}

	IndexBuffer::IndexBuffer(const IndexLayout& layout, size_t indexCount)
		: m_Layout(layout), m_IndexCount(indexCount) {
	}

	IndexBuffer::~IndexBuffer() {}

	IndexLayout IndexBuffer::getIndexLayout() const {
		return m_Layout;
	}

	size_t IndexBuffer::getIndexCount() const {
		return m_IndexCount;
	}

}
