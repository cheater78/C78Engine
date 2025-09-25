#include "C78EPCH.h"
#include "CommandBuffer.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanCommandBuffer.h>

namespace C78E {

	CommandBuffer::CommandBuffer(GraphicsContext& ctx, UsageFlags usage)
		: m_GraphicsContext(ctx), m_State(State::Ready), m_Usage(usage) {
	}

	CommandBuffer::~CommandBuffer() {
	}


	void CommandBuffer::addUsage(UsageFlags usage) {
		m_Usage |= usage;
	}

	//SingleTimeCommandBuffer::SingleTimeCommandBuffer(Ref<CommandBuffer> commandBuffer)
	//	: m_VkCommandBuffer(commandBuffer) {
	//}
	//
	//SingleTimeCommandBuffer::~SingleTimeCommandBuffer() {
	//	m_VkCommandBuffer->getGraphicsContext().submit(m_VkCommandBuffer);
	//}
	//
	//CommandBufferManager::CommandBufferManager(GraphicsContext& graphicsContext)
	//	: m_GraphicsContext(graphicsContext) {
	//}

}
