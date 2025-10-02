#pragma once
#include <C78E/Graphics/API/Buffer/BufferLayout.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	uint32_t findMemoryType(Ref<VulkanDevice> device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkFormat findSupportedFormat(Ref<VulkanDevice> device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkDeviceSize toVkAlignment(VkDeviceSize bufferElementSize, VkDeviceSize minOffsetAlignment);

	class VulkanBuffer {
	public:
		static Scope<VulkanBuffer> create(Ref<VulkanDevice> device,
			size_t size,
			VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags memoryPropertyFlags,
			VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE);
	public:
		VulkanBuffer(Ref<VulkanDevice> device,
			size_t size,
			VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags memoryPropertyFlags,
			VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE);
		virtual ~VulkanBuffer();

		VkDeviceSize getSize() const;
		// to access VulkanBuffer CPU memory the buffer must be host visible and mapped
		bool isHostVisible() const;
		bool isHostCoherent() const;
		// Map to RAM, requires HostVisibility
		bool isMapped() const;
		bool map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0, bool readIn = true);
		void unmap(bool writeBack = false);
		// read/write non HostCoherent memory range
		bool readMappedFromDeviceMemory(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		bool writeMappedToDeviceMemory(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

		VkBuffer getVkBuffer() const;
		const VkBuffer* getVkBufferPtr() const;
		VkBufferUsageFlags getVkUsageFlags() const;
		VkMemoryPropertyFlags getVkMemoryPropertyFlags() const;
		void* getMappedPtr() const;
	protected:
		Ref<VulkanDevice> m_Device;
		VkDeviceSize m_Size = 0;
		VkBufferUsageFlags m_VkBufferUsageFlags = 0;
		VkMemoryPropertyFlags m_VkMemoryPropertyFlags = 0;
		VkSharingMode m_VkSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkBuffer m_VkBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_VkDeviceMemory = VK_NULL_HANDLE;
		// Map to RAM, requires HostCoherence
		void* m_Mapped = nullptr;
		VkDeviceSize m_MappedSize = 0;
		VkDeviceSize m_MappedBufferOffset = 0;
	};

	class VulkanElementBuffer : public VulkanBuffer {
	public:
		VulkanElementBuffer(
			Ref<VulkanDevice> device,
			VkDeviceSize elementSize,
			uint32_t elementCount,
			VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags memoryPropertyFlags,
			VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE);
		virtual ~VulkanElementBuffer();

		VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

		VkDeviceSize getElementSize() const;
		uint32_t getElementCount() const;
		VkDeviceSize getAlignmentSize() const;
	private:
		size_t computeAlignmentSize(VkDeviceSize elementSize, VkBufferUsageFlags usageFlags);
	private:
		VkDeviceSize m_ElementSize;
		VkDeviceSize m_AlignmentSize;
		uint32_t m_ElementCount;
	};

	//TODO: maybe primitive Base Buffer and some structured type? - or could all Buffers be structured?(idea of buffer data structure interface, which requires a layout - constructed on the fly)

}
