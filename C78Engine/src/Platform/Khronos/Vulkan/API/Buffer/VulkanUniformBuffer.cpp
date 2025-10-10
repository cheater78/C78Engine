#include "C78EPCH.h"
#include "VulkanUniformBuffer.h"
#include <Platform/Khronos/Vulkan/Core/VulkanConversions.h>
#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanStagingBuffer.h>

namespace C78E {

	VulkanUniformBuffer::VulkanUniformBuffer(GraphicsContext& ctx, const UniformLayout& layout)
		: GraphicsContextItem(ctx), VulkanGraphicsContextItem(), GPUBuffer(), VulkanGPUBuffer(), UniformBuffer(layout) {
		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer: Creating empty VulkanUniformBuffer..");

		const uint32_t bufferSize = m_Layout.getSize();
		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   Uniform: {} Fields", m_Layout.getFieldCount());
		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   BufferSize: {}", bufferSize);

		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   Creating StagingBuffer..");
		Ref<StagingBuffer> stagingBuffer = createStagingBuffer(ctx, bufferSize);
		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:     Mapping Staging Buffer to RAM..");
		stagingBuffer->map();

		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   Creating VulkanBuffer..");
		m_VulkanBuffer = VulkanBuffer::create(
			getVulkanDevice(),
			bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_SHARING_MODE_EXCLUSIVE //TODO: expose
		);
		C78E_CORE_ASSERT(m_VulkanBuffer, "VulkanUniformBuffer::VulkanUniformBuffer: Failed to create VulkanBuffer!");
	}

	VulkanUniformBuffer::VulkanUniformBuffer(GraphicsContext& ctx, const UniformLayout& layout, Ref<StagingBuffer> stagingBuffer)
		: GraphicsContextItem(ctx), VulkanGraphicsContextItem(), GPUBuffer(), VulkanGPUBuffer(), UniformBuffer(layout) {
		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer: Creating prestaged VulkanUniformBuffer..");
		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   StagingBuffer: Mapped to RAM: {}", stagingBuffer->isMapped());

		const uint32_t uniformSize = m_Layout.getSize();
		const uint32_t bufferSize = stagingBuffer->size();
		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   Uniform: {} Fields", m_Layout.getFieldCount());
		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   BufferSize: {}", bufferSize);
		C78E_CORE_ASSERT(uniformSize == bufferSize, "VulkanUniformBuffer::VulkanUniformBuffer:  stagingBuffer was not the same size as layout!");

		setStagingBuffer(stagingBuffer);

		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   Creating VulkanBuffer..");
		m_VulkanBuffer = VulkanBuffer::create(
			getVulkanDevice(),
			stagingBuffer->size(),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_SHARING_MODE_EXCLUSIVE //TODO: expose
		);
		C78E_CORE_ASSERT(m_VulkanBuffer, "VulkanUniformBuffer::VulkanUniformBuffer: Failed to create VulkanBuffer!");

		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer:   Copying StagingBuffer to VertexBuffer..");
		ctx.copyBuffer(*stagingBuffer, *this);

		C78E_CORE_TRACE("VulkanUniformBuffer::VulkanUniformBuffer: Created VulkanUniformBuffer.");
	}

	VulkanUniformBuffer::~VulkanUniformBuffer() {
	}

}
