#include "vulkanBuffer.h"

#include <cassert>
#include <cstring>

namespace C78e {
	/**
	 * Returns the minimum instance size required to be compatible with devices minOffsetAlignment
	 *
	 * @param m_InstanceSize The size of an instance
	 * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
	 * minUniformBufferOffsetAlignment)
	 *
	 * @return VkResult of the buffer mapping call
	 */
	VkDeviceSize VulkanBuffer::getAlignment(VkDeviceSize m_InstanceSize, VkDeviceSize minOffsetAlignment) {
		if (minOffsetAlignment > 0) {
			return (m_InstanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		}
		return m_InstanceSize;
	}

	VulkanBuffer::VulkanBuffer(VulkanDevice& device, VkDeviceSize m_InstanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
			: m_Device{device}, m_InstanceSize{m_InstanceSize}, m_InstanceCount{instanceCount}, m_UsageFlags{usageFlags}, m_MemoryPropertyFlags{memoryPropertyFlags} {
		m_AlignmentSize = getAlignment(m_InstanceSize, minOffsetAlignment);
		m_BufferSize = m_AlignmentSize * instanceCount;
		device.createBuffer(m_BufferSize, usageFlags, memoryPropertyFlags, m_Buffer, m_Memory);
	}

	VulkanBuffer::~VulkanBuffer() {
		unmap();
		vkDestroyBuffer(m_Device.device(), m_Buffer, nullptr);
		vkFreeMemory(m_Device.device(), m_Memory, nullptr);
	}

	/**
	 * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
	 *
	 * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
	 * buffer range.
	 * @param offset (Optional) Byte offset from beginning
	 *
	 * @return VkResult of the buffer mapping call
	 */
	VkResult VulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
		assert(m_Buffer && m_Memory && "Called map on buffer before create");

		return vkMapMemory(m_Device.device(), m_Memory, offset, size, 0, &m_Mapped);
	}

	/**
	 * Unmap a mapped memory range
	 *
	 * @note Does not return a result as vkUnmapMemory can't fail
	 */
	void VulkanBuffer::unmap() {
		if (m_Mapped) {
			vkUnmapMemory(m_Device.device(), m_Memory);
			m_Mapped = nullptr;
		}
	}

	/**
	 * Copies the specified data to the mapped buffer. Default value writes whole buffer range
	 *
	 * @param data Pointer to the data to copy
	 * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
	 * range.
	 * @param offset (Optional) Byte offset from beginning of mapped region
	 *
	 */
	void VulkanBuffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset) {
		assert(m_Mapped && "Cannot copy to unmapped buffer");

		if (size == VK_WHOLE_SIZE) {
			memcpy(m_Mapped, data, m_BufferSize);
		} else {
			char *memOffset = (char *)m_Mapped;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}

	/**
		* Flush a memory range of the buffer to make it visible to the device
		*
		* @note Only required for non-coherent memory
		*
		* @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
		* complete buffer range.
		* @param offset (Optional) Byte offset from beginning
		*
		* @return VkResult of the flush call
		*/
	VkResult VulkanBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_Memory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		return vkFlushMappedMemoryRanges(m_Device.device(), 1, &mappedRange);
	}

	/**
	 * Invalidate a memory range of the buffer to make it visible to the host
	 *
	 * @note Only required for non-coherent memory
	 *
	 * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
	 * the complete buffer range.
	 * @param offset (Optional) Byte offset from beginning
	 *
	 * @return VkResult of the invalidate call
	 */
	VkResult VulkanBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_Memory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		return vkInvalidateMappedMemoryRanges(m_Device.device(), 1, &mappedRange);
	}

	/**
	 * Create a buffer info descriptor
	 *
	 * @param size (Optional) Size of the memory range of the descriptor
	 * @param offset (Optional) Byte offset from beginning
	 *
	 * @return VkDescriptorBufferInfo of specified offset and range
	 */
	VkDescriptorBufferInfo VulkanBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) { return VkDescriptorBufferInfo{ m_Buffer, offset, size }; }

	/**
	 * Copies "m_InstanceSize" bytes of data to the mapped buffer at an offset of index * m_AlignmentSize
	 *
	 * @param data Pointer to the data to copy
	 * @param index Used in offset calculation
	 *
	 */
	void VulkanBuffer::writeToIndex(void *data, int index) {
		writeToBuffer(data, m_InstanceSize, index * m_AlignmentSize);
	}

	/**
	 *Flush the memory range at index * m_AlignmentSize of the buffer to make it visible to the device
	 *
	 * @param index Used in offset calculation
	 *
	 */
	VkResult VulkanBuffer::flushIndex(int index) { return flush(m_AlignmentSize, index * m_AlignmentSize); }

	/**
	 * Create a buffer info descriptor
	 *
	 * @param index Specifies the region given by index * m_AlignmentSize
	 *
	 * @return VkDescriptorBufferInfo for instance at index
	 */
	VkDescriptorBufferInfo VulkanBuffer::descriptorInfoForIndex(int index) {
		return descriptorInfo(m_AlignmentSize, index * m_AlignmentSize);
	}

	/**
	 * Invalidate a memory range of the buffer to make it visible to the host
	 *
	 * @note Only required for non-coherent memory
	 *
	 * @param index Specifies the region to invalidate: index * m_AlignmentSize
	 *
	 * @return VkResult of the invalidate call
	 */
	VkResult VulkanBuffer::invalidateIndex(int index) {
		return invalidate(m_AlignmentSize, index * m_AlignmentSize);
	}

}
