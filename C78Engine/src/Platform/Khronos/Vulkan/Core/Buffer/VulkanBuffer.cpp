#include "C78EPCH.h"
#include "VulkanBuffer.h"

namespace C78E {
	
	uint32_t findMemoryType(Ref<VulkanDevice> device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(device->getVkPhysicalDevice(), &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}
		C78E_CORE_ASSERT(false, "VulkanBufferManager::findMemoryType: Failed to find suitable memory type!");
	}
	VkFormat findSupportedFormat(Ref<VulkanDevice> device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(device->getVkPhysicalDevice(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}
		throw std::runtime_error("failed to find supported format!");
	}

	VkDeviceSize toVkAlignment(VkDeviceSize bufferElementSize, VkDeviceSize minOffsetAlignment) {
		if(minOffsetAlignment > 0) {
			return (bufferElementSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		}
		return bufferElementSize;
	}

	VulkanBuffer::VulkanBuffer(Ref<VulkanDevice> device, VkDeviceSize elementSize, uint32_t elementCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
		: m_Device(device), m_ElementSize(elementSize), m_ElementCount(elementCount), m_UsageFlags(usageFlags), m_MemoryPropertyFlags(memoryPropertyFlags), m_AlignmentSize(1) {
		const VkPhysicalDeviceProperties& properties = m_Device->getPhysicalDeviceProperties();
		const VkPhysicalDeviceLimits& limits = properties.limits;
		
		if (usageFlags & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
			m_AlignmentSize = toVkAlignment(limits.minUniformBufferOffsetAlignment, m_AlignmentSize);
		}
		if (usageFlags & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) {
			m_AlignmentSize = toVkAlignment(limits.minStorageBufferOffsetAlignment, m_AlignmentSize);
		}
		if (usageFlags & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) {
			m_AlignmentSize = toVkAlignment(limits.minMemoryMapAlignment, m_AlignmentSize);
		}

		C78E_CORE_VALIDATE(createBuffer(), return, "VulkanBuffer::VulkanBuffer: Failed to create Vulkan buffer!");
	}
	VulkanBuffer::~VulkanBuffer() {
		destroyBuffer();
	}

	inline VkBuffer VulkanBuffer::getVkBuffer() const {
		return m_Buffer;
	}
	const VkBuffer* VulkanBuffer::getVkBufferPtr() const {
		return &m_Buffer;
	}
	inline VkDeviceSize VulkanBuffer::getElementSize() const {
		return m_ElementSize;
	}
	inline uint32_t VulkanBuffer::getElementCount() const {
		return m_ElementCount;
	}
	inline VkBufferUsageFlags VulkanBuffer::getVkUsageFlags() const {
		return m_UsageFlags;
	}
	inline VkMemoryPropertyFlags VulkanBuffer::getVkMemoryPropertyFlags() const {
		return m_MemoryPropertyFlags;
	}
	inline VkDeviceSize VulkanBuffer::getAlignmentSize() const {
		return m_AlignmentSize;
	}

	inline VkDeviceSize VulkanBuffer::getBufferSize() const {
		return m_ElementCount * m_AlignmentSize;
	}
	inline VkDescriptorBufferInfo VulkanBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
		return VkDescriptorBufferInfo{ m_Buffer, offset, size };
	}
	

	inline bool VulkanBuffer::isHostVisible() const {
		return (m_MemoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}
	inline bool VulkanBuffer::isHostCoherent() const { return (m_MemoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); }
	inline bool VulkanBuffer::isMapped() const { return m_Mapped != nullptr; }

	bool VulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset, bool readIn) {
		if (isMapped()) {
			unmap();
		}
		m_MappedSize = size;
		m_MappedBufferOffset = offset;
		VkResult result = vkMapMemory(m_Device->getVkDevice(), m_Memory, m_MappedBufferOffset, m_MappedSize, 0, &m_Mapped);
		C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanBuffer::map: Failed to map buffer memory!");
		if (readIn && !isHostCoherent()) {
			readMappedFromDeviceMemory();
		}
		return true;
	}
	void VulkanBuffer::unmap(bool writeBack) {
		if (!m_Mapped) {
			C78E_CORE_WARN("VulkanBuffer::unmap: Buffer is not mapped, nothing to unmap!");
			return;
		}

		if (writeBack && !isHostCoherent()) {
			writeMappedToDeviceMemory();
		}
		vkUnmapMemory(m_Device->getVkDevice(), m_Memory);
		m_Mapped = nullptr;
		m_MappedSize = 0;
		m_MappedBufferOffset = 0;
	}

	bool VulkanBuffer::readMappedFromDeviceMemory(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_Memory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		VkResult result = vkInvalidateMappedMemoryRanges(m_Device->getVkDevice(), 1, &mappedRange);
		C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanBuffer::readMappedFromDeviceMemory: Failed to invalidate mapped memory ranges!");
		return true;
	}
	bool VulkanBuffer::writeMappedToDeviceMemory(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_Memory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		VkResult result = vkFlushMappedMemoryRanges(m_Device->getVkDevice(), 1, &mappedRange);
		C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanBuffer::writeMappedToDeviceMemory: Failed to flush mapped memory ranges!");
		return true;
	}

	bool VulkanBuffer::createBuffer() {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = getBufferSize();
		bufferInfo.usage = m_UsageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult createBufferResult = vkCreateBuffer(m_Device->getVkDevice(), &bufferInfo, nullptr, &m_Buffer);
		C78E_CORE_VALIDATE(createBufferResult == VK_SUCCESS, return false, "VulkanBufferManager::createVulkanBuffer: Failed to create vulkan buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_Device->getVkDevice(), m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(m_Device, memRequirements.memoryTypeBits, m_MemoryPropertyFlags);

		VkResult allocateMemoryResult = vkAllocateMemory(m_Device->getVkDevice(), &allocInfo, nullptr, &m_Memory);
		C78E_CORE_VALIDATE(allocateMemoryResult == VK_SUCCESS, return false, "VulkanBufferManager::createVulkanBuffer: Failed to allocate buffer memory!");

		VkResult bindMemoryResult = vkBindBufferMemory(m_Device->getVkDevice(), m_Buffer, m_Memory, 0);
		C78E_CORE_VALIDATE(bindMemoryResult == VK_SUCCESS, return false, "VulkanBufferManager::createVulkanBuffer: Failed to bind buffer memory!");

		return true;
	}

	void VulkanBuffer::destroyBuffer() {
		if(m_Mapped) {
			vkUnmapMemory(m_Device->getVkDevice(), m_Memory);
			m_Mapped = nullptr;
		}
		if(m_Buffer) {
			vkDestroyBuffer(m_Device->getVkDevice(), m_Buffer, nullptr);
			m_Buffer = VK_NULL_HANDLE;
		}
		if(m_Memory) {
			vkFreeMemory(m_Device->getVkDevice(), m_Memory, nullptr);
			m_Memory = VK_NULL_HANDLE;
		}
	}

}
