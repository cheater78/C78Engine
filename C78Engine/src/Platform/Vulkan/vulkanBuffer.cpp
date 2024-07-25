#include "C78EPCH.h"
#include "VulkanBuffer.h"

namespace C78E {

	/////////////////////////////////////////////////////////////////////////////
	// abstract VulkanBuffer ////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	VulkanBuffer::VulkanBuffer(VkDeviceSize m_InstanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
		: m_Device(VulkanDevice::get()), m_InstanceSize{ m_InstanceSize }, m_InstanceCount{ instanceCount }, m_UsageFlags{ usageFlags }, m_MemoryPropertyFlags{ memoryPropertyFlags } {
		m_AlignmentSize = getAlignment(m_InstanceSize, minOffsetAlignment);
		m_BufferSize = m_AlignmentSize * instanceCount;
		m_Device.createBuffer(m_BufferSize, usageFlags, memoryPropertyFlags, m_Buffer, m_Memory);
	}

	VulkanBuffer::~VulkanBuffer() {
		unmap();
		vkDestroyBuffer(m_Device.device(), m_Buffer, nullptr);
		vkFreeMemory(m_Device.device(), m_Memory, nullptr);
	}

	VkResult VulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
		assert(m_Buffer && m_Memory && "Called map on buffer before create");

		return vkMapMemory(m_Device.device(), m_Memory, offset, size, 0, &m_Mapped);
	}

	void VulkanBuffer::unmap() {
		if (m_Mapped) {
			vkUnmapMemory(m_Device.device(), m_Memory);
			m_Mapped = nullptr;
		}
	}

	void VulkanBuffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
		assert(m_Mapped && "Cannot copy to unmapped buffer");

		if (size == VK_WHOLE_SIZE) {
			memcpy(m_Mapped, data, m_BufferSize);
		}
		else {
			char* memOffset = (char*)m_Mapped;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}

	VkResult VulkanBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_Memory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		return vkFlushMappedMemoryRanges(m_Device.device(), 1, &mappedRange);
	}

	VkResult VulkanBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_Memory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		return vkInvalidateMappedMemoryRanges(m_Device.device(), 1, &mappedRange);
	}

	VkDescriptorBufferInfo VulkanBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) { return VkDescriptorBufferInfo{ m_Buffer, offset, size }; }

	void VulkanBuffer::writeToIndex(void* data, int index) {
		writeToBuffer(data, m_InstanceSize, index * m_AlignmentSize);
	}

	VkResult VulkanBuffer::flushIndex(int index) { return flush(m_AlignmentSize, index * m_AlignmentSize); }

	VkDescriptorBufferInfo VulkanBuffer::descriptorInfoForIndex(int index) {
		return descriptorInfo(m_AlignmentSize, index * m_AlignmentSize);
	}

	VkResult VulkanBuffer::invalidateIndex(int index) {
		return invalidate(m_AlignmentSize, index * m_AlignmentSize);
	}

	VkDeviceSize VulkanBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
		return (minOffsetAlignment > 0) ? (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1) : instanceSize;
	}

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size)
		: VulkanBuffer(size, 1, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0) {
	}

	VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size)
		: VulkanBuffer(size, 1, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0) {
		map();
		writeToBuffer(vertices, size, 0);
		unmap();
	}

	VulkanVertexBuffer::~VulkanVertexBuffer() {
	}

	void VulkanVertexBuffer::bind() const {
		VkBuffer buffers[] = { m_Buffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void VulkanVertexBuffer::unbind() const {
	}

	void VulkanVertexBuffer::setData(const void* data, uint32_t size) {
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count)
		: VulkanBuffer(count, 1, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0), m_Count(count) {
	}

	VulkanIndexBuffer::~VulkanIndexBuffer() {
	}

	void VulkanIndexBuffer::bind() const {
		vkCmdBindIndexBuffer(commandBuffer, m_Buffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanIndexBuffer::unbind() const {
	}

}
