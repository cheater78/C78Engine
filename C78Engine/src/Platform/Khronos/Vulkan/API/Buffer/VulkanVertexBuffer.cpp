#include "C78EPCH.h"
#include "VulkanVertexBuffer.h"

namespace C78E {

	VulkanVertexBuffer::VulkanVertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, size_t vertexCount)
		: VertexBuffer(layout, data->size() / layout.getStride()) {
		C78E_CORE_ASSERT(m_VertexCount >= 3 && "VulkanVertexBuffer::VulkanVertexBuffer: less than 3 Vertecies supplied!");

		VkDeviceSize bufferSize = sizeof(vertecies[0]) * vertexCount;
		uint32_t vertexSize = sizeof(vertecies[0]);

		Buffer stagingBuffer{
			vulkDevice,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)vertecies.data());

		vertexBuffer = make_unique<Buffer>(
			vulkDevice,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		vulkDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);

		//
		m_VertexBuffer = VulkanBuffer::create(
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

	}

	VulkanVertexBuffer::VulkanVertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, Ref<StagingBuffer> stagingBuffer)
		: VertexBuffer() {

	}

	VulkanVertexBuffer::~VulkanVertexBuffer() {}

	inline Ref<VulkanBuffer> VulkanVertexBuffer::getVertexBuffer() const {
		return m_VertexBuffer;
	}

}
