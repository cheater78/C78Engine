#include "C78EPCH.h"
#include "StagingBuffer.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanStagingBuffer.h>

namespace C78E {

	Ref<StagingBuffer> StagingBuffer::create(GraphicsContext& ctx, size_t size) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanStagingBuffer>(ctx, size);
		default:
			C78E_CORE_ASSERT(false, "StagingBuffer::create: Unsupported API!");
		}
		return nullptr;
	}

	bool StagedBuffer::hasStagingBuffer() const {
		return m_StagingBuffer != nullptr;
	}

	Ref<StagingBuffer> StagedBuffer::getStagingBuffer() const {
		return m_StagingBuffer;
	}

	Ref<StagingBuffer> StagedBuffer::setStagingBuffer(Ref<StagingBuffer> stagingBuffer) {
		return m_StagingBuffer = stagingBuffer;
	}

	Ref<StagingBuffer> StagedBuffer::createStagingBuffer(GraphicsContext& ctx, size_t size) {
		return m_StagingBuffer = StagingBuffer::create(ctx, size); // (re-)create -> impl. must be owning
	}

	void StagedBuffer::dropStagingBuffer() {
		m_StagingBuffer = nullptr;
	}

}
