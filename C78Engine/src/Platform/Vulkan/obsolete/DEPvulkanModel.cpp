#include "vulkanModel.h"

#include <cstring>
#include <iostream>

namespace C78E {
	
	VulkanModel::VulkanModel(VulkanDevice& device, std::vector<Vertex>& vertecies, std::vector<uint32_t>& indecies) : m_Device{ device } {
		createBuffers(vertecies, indecies);
	}
	
	VulkanModel::~VulkanModel(){
	}


	void VulkanModel::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { m_VertexBuffer->getBuffer()};
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer())
			vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanModel::draw(VkCommandBuffer commandBuffer) {
		if (hasIndexBuffer())
			vkCmdDrawIndexed(commandBuffer, m_IndexCount, 1, 0, 0, 0);
		else
			vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
	}

	void VulkanModel::createBuffers(std::vector<Vertex>& vertecies, std::vector<uint32_t>& indecies) {

		{ // Create Vertex Buffer
			m_VertexCount = static_cast<uint32_t>(vertecies.size());
			assert(m_VertexCount >= 3 && "too few vertecies");

			uint32_t vertexSize = sizeof(vertecies[0]);
			VkDeviceSize bufferSize = static_cast<VkDeviceSize>(vertexSize * m_VertexCount);
		
			VulkanBuffer stagingBuffer{
				m_Device,
				vertexSize,
				m_VertexCount,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			};

			stagingBuffer.map();
			stagingBuffer.writeToBuffer((void*)vertecies.data());

			m_VertexBuffer = std::make_unique<VulkanBuffer>(
				m_Device,
				vertexSize,
				m_VertexCount,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			m_Device.copyBuffer(stagingBuffer.getBuffer(), m_VertexBuffer->getBuffer(), bufferSize);
		}

		{ // Create Index Buffer
			if (indecies.size() == 0) return;

			m_IndexCount = static_cast<uint32_t>(indecies.size());

			uint32_t indexSize = sizeof(indecies[0]);
			VkDeviceSize bufferSize = static_cast<VkDeviceSize>(indexSize * m_IndexCount);

			VulkanBuffer stagingBuffer{
				m_Device,
				indexSize,
				m_IndexCount,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			};

			stagingBuffer.map();
			stagingBuffer.writeToBuffer((void*)indecies.data());


			m_IndexBuffer = std::make_unique<VulkanBuffer>(
				m_Device,
				indexSize,
				m_IndexCount,
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			m_Device.copyBuffer(stagingBuffer.getBuffer(), m_IndexBuffer->getBuffer(), bufferSize);
		}
	}
}