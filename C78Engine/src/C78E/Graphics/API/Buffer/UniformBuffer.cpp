#include "C78EPCH.h"
#include "UniformBuffer.h"
#include <C78E/Graphics/API/GraphicsContext.h>
#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanUniformBuffer.h>


namespace C78E {

	Ref<UniformBuffer> UniformBuffer::create(GraphicsContext& ctx, const UniformLayout& layout) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanUniformBuffer>(ctx, layout);
		default:
			C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		}
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::create(GraphicsContext& ctx, const UniformLayout& layout, Ref<StagingBuffer> stagingBuffer) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanUniformBuffer>(ctx, layout, stagingBuffer);
		default:
			C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		}
		return nullptr;
	}

	UniformBuffer::UniformBuffer(const UniformLayout& layout)
		: m_Layout(layout) {
	}

	UniformBuffer::~UniformBuffer() {
	}

	const UniformLayout UniformBuffer::getUniformLayout() const {
		return m_Layout;
	}

}
