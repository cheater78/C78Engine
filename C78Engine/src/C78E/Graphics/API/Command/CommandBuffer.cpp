#include "C78EPCH.h"
#include "CommandBuffer.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanCommandBuffer.h>

namespace C78E {

	CommandBuffer::CommandBuffer(bool reusable, UsageFlags usage)
		: m_Reusable(reusable), m_State(State::Ready), m_Usage(usage) {
	}

	CommandBuffer::~CommandBuffer() {
	}


	void CommandBuffer::addUsage(UsageFlags usage) {
		m_Usage |= usage;
	}

}
