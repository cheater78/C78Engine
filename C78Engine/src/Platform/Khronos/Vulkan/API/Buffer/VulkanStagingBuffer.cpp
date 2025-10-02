#include "C78EPCH.h"
#include "VulkanStagingBuffer.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>

namespace C78E {

	VulkanStagingBuffer::VulkanStagingBuffer(GraphicsContext& ctx, size_t size)
		: GraphicsContextItem(ctx),
		VulkanGraphicsContextItem(),
		GPUBuffer(),
		VulkanGPUBuffer(),
		StagingBuffer() {
		VulkanGraphicsContext& vulkanGraphicsContext = ctx.getAs<VulkanGraphicsContext>();
		m_Device = vulkanGraphicsContext.getDevice();
		m_VulkanBuffer = createScope<VulkanBuffer>(
			m_Device,
			size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_SHARING_MODE_EXCLUSIVE //TODO: expose
		);
	}

	VulkanStagingBuffer::~VulkanStagingBuffer() {
		m_VulkanBuffer = nullptr;
	}

	bool VulkanStagingBuffer::isMapped() const {
		return m_VulkanBuffer->isMapped();
	}

	bool VulkanStagingBuffer::map() {
		return m_VulkanBuffer->map();
	}

	void VulkanStagingBuffer::unmap(bool writeBack) {
		m_VulkanBuffer->unmap(writeBack);
	}

	void* VulkanStagingBuffer::data() {
		return m_VulkanBuffer->getMappedPtr();
	}

	size_t VulkanStagingBuffer::size() {
		return m_VulkanBuffer->getSize();;
	}


}

