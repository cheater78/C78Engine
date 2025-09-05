#pragma once
#include <C78E/Graphics/API/Buffer/BufferLayout.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	uint32_t findMemoryType(Ref<VulkanDevice> device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkFormat findSupportedFormat(Ref<VulkanDevice> device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkDeviceSize toVkAlignment(VkDeviceSize bufferElementSize, VkDeviceSize minOffsetAlignment);

	class VulkanBuffer {
	public:
		VulkanBuffer(
			Ref<VulkanDevice> device,
			VkDeviceSize elementSize,
			uint32_t elementCount,
			VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags memoryPropertyFlags);
		virtual ~VulkanBuffer();

	public:
		VkBuffer getVkBuffer() const;
		const VkBuffer* getVkBufferPtr() const;
		VkDeviceSize getElementSize() const;
		uint32_t getElementCount() const;
		VkBufferUsageFlags getVkUsageFlags() const;
		VkMemoryPropertyFlags getVkMemoryPropertyFlags() const;
		VkDeviceSize getAlignmentSize() const;

		VkDeviceSize getBufferSize() const;
		VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	public: // to access VulkanBuffer CPU memory the buffer must be host visible
		bool isHostVisible() const;
		bool isHostCoherent() const;
		bool isMapped() const;

		bool map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0, bool readIn = false);
		void unmap(bool writeBack = false);

		bool readMappedFromDeviceMemory(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		bool writeMappedToDeviceMemory(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	private:
		bool createBuffer();
		void destroyBuffer();
	private:
		Ref<VulkanDevice> m_Device;
		VkDeviceSize m_ElementSize;
		uint32_t m_ElementCount;
		VkBufferUsageFlags m_UsageFlags;
		VkMemoryPropertyFlags m_MemoryPropertyFlags;
		VkDeviceSize m_AlignmentSize;

		VkBuffer m_Buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_Memory = VK_NULL_HANDLE;

		void* m_Mapped = nullptr;
		VkDeviceSize m_MappedSize = 0;
		VkDeviceSize m_MappedBufferOffset = 0;

	};

}
