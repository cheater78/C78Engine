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
		return -1;
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


	Scope<VulkanBuffer> VulkanBuffer::create(Ref<VulkanDevice> device, size_t size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkSharingMode sharingMode) {
		return createScope<VulkanBuffer>(device, size, usageFlags, memoryPropertyFlags, sharingMode);
	}

	VulkanBuffer::VulkanBuffer(Ref<VulkanDevice> device, size_t size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkSharingMode sharingMode)
		: m_Device(device), m_Size(size), m_VkBufferUsageFlags(usageFlags), m_VkMemoryPropertyFlags(memoryPropertyFlags), m_VkSharingMode(sharingMode) {
		const VkDeviceSize bufferSize = static_cast<VkDeviceSize>(size);

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = sharingMode;

		VkResult createBufferResult = vkCreateBuffer(m_Device->getVkDevice(), &bufferInfo, nullptr, &m_VkBuffer);
		C78E_CORE_ASSERT(createBufferResult == VK_SUCCESS, "VulkanBufferManager::createVulkanBuffer: Failed to create vulkan buffer!");

		VkMemoryRequirements memRequirements; // query VkMemoryRequirements
		vkGetBufferMemoryRequirements(m_Device->getVkDevice(), m_VkBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(m_Device, memRequirements.memoryTypeBits, memoryPropertyFlags);

		VkResult allocateMemoryResult = vkAllocateMemory(m_Device->getVkDevice(), &allocInfo, nullptr, &m_VkDeviceMemory);
		C78E_CORE_ASSERT(allocateMemoryResult == VK_SUCCESS, "VulkanBufferManager::createVulkanBuffer: Failed to allocate buffer memory!");

		VkResult bindMemoryResult = vkBindBufferMemory(m_Device->getVkDevice(), m_VkBuffer, m_VkDeviceMemory, 0);
		C78E_CORE_ASSERT(bindMemoryResult == VK_SUCCESS, "VulkanBufferManager::createVulkanBuffer: Failed to bind buffer memory!");

	}

	VulkanBuffer::~VulkanBuffer() {
		if (m_Mapped) {
			vkUnmapMemory(m_Device->getVkDevice(), m_VkDeviceMemory);
			m_Mapped = nullptr;
		}
		if (m_VkBuffer) {
			vkDestroyBuffer(m_Device->getVkDevice(), m_VkBuffer, nullptr);
			m_VkBuffer = VK_NULL_HANDLE;
		}
		if (m_VkDeviceMemory) {
			vkFreeMemory(m_Device->getVkDevice(), m_VkDeviceMemory, nullptr);
			m_VkDeviceMemory = VK_NULL_HANDLE;
		}
	}

	VkDeviceSize VulkanBuffer::getSize() const {
		return m_Size;
	}

	VkBuffer VulkanBuffer::getVkBuffer() const {
		return m_VkBuffer;
	}

	const VkBuffer* VulkanBuffer::getVkBufferPtr() const {
		return &m_VkBuffer;
	}

	VkBufferUsageFlags VulkanBuffer::getVkUsageFlags() const {
		return m_VkBufferUsageFlags;
	}

	VkMemoryPropertyFlags VulkanBuffer::getVkMemoryPropertyFlags() const {
		return m_VkMemoryPropertyFlags;
	}

	void* VulkanBuffer::getMappedPtr() const {
		return m_Mapped;
	}

	inline bool VulkanBuffer::isHostVisible() const {
		return (m_VkMemoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}
	inline bool VulkanBuffer::isHostCoherent() const { return (m_VkMemoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); }
	inline bool VulkanBuffer::isMapped() const { return m_Mapped != nullptr; }

	bool VulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset, bool readIn) {
		if (isMapped()) {
			unmap();
		}
		m_MappedSize = size;
		m_MappedBufferOffset = offset;
		VkResult result = vkMapMemory(m_Device->getVkDevice(), m_VkDeviceMemory, m_MappedBufferOffset, m_MappedSize, 0, &m_Mapped);
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
		vkUnmapMemory(m_Device->getVkDevice(), m_VkDeviceMemory);
		m_Mapped = nullptr;
		m_MappedSize = 0;
		m_MappedBufferOffset = 0;
	}

	bool VulkanBuffer::readMappedFromDeviceMemory(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_VkDeviceMemory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		VkResult result = vkInvalidateMappedMemoryRanges(m_Device->getVkDevice(), 1, &mappedRange);
		C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanBuffer::readMappedFromDeviceMemory: Failed to invalidate mapped memory ranges!");
		return true;
	}
	bool VulkanBuffer::writeMappedToDeviceMemory(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_VkDeviceMemory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		VkResult result = vkFlushMappedMemoryRanges(m_Device->getVkDevice(), 1, &mappedRange);
		C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanBuffer::writeMappedToDeviceMemory: Failed to flush mapped memory ranges!");
		return true;
	}



	VulkanElementBuffer::VulkanElementBuffer(
		Ref<VulkanDevice> device,
		VkDeviceSize elementSize,
		uint32_t elementCount,
		VkBufferUsageFlags usageFlags,
		VkMemoryPropertyFlags memoryPropertyFlags,
		VkSharingMode sharingMode)
		: VulkanBuffer(device, elementCount * computeAlignmentSize(elementSize, usageFlags), usageFlags, memoryPropertyFlags, sharingMode),
		m_ElementSize(elementSize),
		m_ElementCount(elementCount) {
	}

	VulkanElementBuffer::~VulkanElementBuffer() {
	}
	
	VkDeviceSize VulkanElementBuffer::getElementSize() const {
		return m_ElementSize;
	}
	uint32_t VulkanElementBuffer::getElementCount() const {
		return m_ElementCount;
	}
	
	VkDeviceSize VulkanElementBuffer::getAlignmentSize() const {
		return m_AlignmentSize;
	}
	
	VkDescriptorBufferInfo VulkanElementBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
		return VkDescriptorBufferInfo{ m_VkBuffer, offset, size };
	}

	size_t VulkanElementBuffer::computeAlignmentSize(VkDeviceSize elementSize, VkBufferUsageFlags usageFlags) {
		const VkPhysicalDeviceProperties& properties = m_Device->getPhysicalDeviceProperties();
		const VkPhysicalDeviceLimits& limits = properties.limits;
		
		VkDeviceSize minAlignmentSize = 1;
		if(usageFlags & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
			minAlignmentSize = std::max(minAlignmentSize, limits.minUniformBufferOffsetAlignment);
		}
		if(usageFlags & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) {
			minAlignmentSize = std::max(minAlignmentSize, limits.minStorageBufferOffsetAlignment);
		}
		if(usageFlags & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) {
			minAlignmentSize = std::max(minAlignmentSize, limits.minMemoryMapAlignment);
		}

		m_AlignmentSize = toVkAlignment(elementSize, minAlignmentSize);

		return static_cast<VkDeviceSize>(m_AlignmentSize);
	}

}
