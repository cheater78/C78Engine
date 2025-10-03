#include "C78EPCH.h"
#include "VulkanIndexBuffer.h"
#include <Platform/Khronos/Vulkan/Core/VulkanConversions.h>
#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanStagingBuffer.h>

namespace C78E {

	VkIndexType toVkIndexType(const IndexLayout& layout) {
		switch (layout.raw()) {
		case IndexLayout::UInt8: return VK_INDEX_TYPE_UINT8;
		case IndexLayout::UInt16: return VK_INDEX_TYPE_UINT16;
		case IndexLayout::UInt32: return VK_INDEX_TYPE_UINT32;
		default:
			C78E_CORE_ERROR("toVkIndexType: layout was not a valid IndexLayout!");
		}
		return VK_INDEX_TYPE_UINT32;
	}

	VulkanIndexBuffer::VulkanIndexBuffer(GraphicsContext& ctx, const IndexLayout& layout, size_t indexCount)
		: GraphicsContextItem(ctx), VulkanGraphicsContextItem(), GPUBuffer(), VulkanGPUBuffer(), IndexBuffer(layout, indexCount) {
		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer: Creating empty VulkanIndexBuffer..");

		const uint32_t indexSize = static_cast<uint32_t>(m_Layout.size());
		const uint32_t bufferSize = static_cast<uint32_t>(indexCount) * indexSize;
		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   Index: Size: {}, Count: {}", indexSize, indexCount);
		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   BufferSize: {}", bufferSize);

		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   Creating StagingBuffer..");
		Ref<StagingBuffer> stagingBuffer = createStagingBuffer(ctx, bufferSize);
		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:     Mapping Staging Buffer to RAM..");
		stagingBuffer->map();

		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   Creating VulkanBuffer..");
		m_VulkanBuffer = VulkanBuffer::create(
			getVulkanDevice(),
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_SHARING_MODE_EXCLUSIVE //TODO: expose
		);
		C78E_CORE_ASSERT(m_VulkanBuffer, "VulkanIndexBuffer::VulkanIndexBuffer: Failed to create VulkanBuffer!");
	}

	VulkanIndexBuffer::VulkanIndexBuffer(GraphicsContext& ctx, const IndexLayout& layout, Ref<StagingBuffer> stagingBuffer)
		: GraphicsContextItem(ctx), VulkanGraphicsContextItem(), GPUBuffer(), VulkanGPUBuffer(), IndexBuffer(layout, stagingBuffer->size() / layout.size()) {
		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer: Creating prestaged VulkanIndexBuffer..");
		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   StagingBuffer: Mapped to RAM: {}", stagingBuffer->isMapped());
		
		const uint32_t indexSize = static_cast<uint32_t>(m_Layout.size());
		const uint32_t bufferSize = static_cast<uint32_t>(stagingBuffer->size());
		const size_t indexCount = bufferSize / indexSize;
		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   Index: Size: {}, Count: {}", indexSize, indexCount);
		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   BufferSize: {}", bufferSize);

		setStagingBuffer(stagingBuffer);

		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   Creating VulkanBuffer..");
		m_VulkanBuffer = VulkanBuffer::create(
			getVulkanDevice(),
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_SHARING_MODE_EXCLUSIVE //TODO: expose
		);
		C78E_CORE_ASSERT(m_VulkanBuffer, "VulkanIndexBuffer::VulkanIndexBuffer: Failed to create VulkanBuffer!");

		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer:   Copying StagingBuffer to IndexBuffer..");
		ctx.copyBuffer(*stagingBuffer, *this);

		C78E_CORE_TRACE("VulkanIndexBuffer::VulkanIndexBuffer: Created VulkanIndexBuffer.");
	}

	VulkanIndexBuffer::~VulkanIndexBuffer() {
	}

}
