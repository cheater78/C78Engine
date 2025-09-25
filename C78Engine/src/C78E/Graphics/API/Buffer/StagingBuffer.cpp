#include "C78EPCH.h"
#include "StagingBuffer.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanStagingBuffer.h>

namespace C78E {

	Ref<StagingBuffer> StagingBuffer::create(size_t size) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanStagingBuffer>(size);
		default:
			C78E_CORE_ASSERT(false, "StagingBuffer::create: Unsupported API!");
		}
		return Ref<StagingBuffer>();
	}

}
