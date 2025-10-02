#include "C78EPCH.h"
#include "VulkanVertexBuffer.h"
#include <Platform/Khronos/Vulkan/Core/VulkanConversions.h>
#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanStagingBuffer.h>

namespace C78E {

	VkVertexInputRate toVkVertexInputRate(VertexInputRate inputRate) {
		switch (inputRate) {
		case VertexInputRate::Instance: return VK_VERTEX_INPUT_RATE_INSTANCE;
		case VertexInputRate::Vertex:	return VK_VERTEX_INPUT_RATE_VERTEX;
		default:						return VK_VERTEX_INPUT_RATE_VERTEX;
		}
	}

	VulkanVertexBuffer::VulkanVertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, size_t vertexCount)
		: GraphicsContextItem(ctx), VulkanGraphicsContextItem(), GPUBuffer(), VulkanGPUBuffer(), VertexBuffer(inputRate, layout, vertexCount) {
		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer: Creating empty VulkanVertexBuffer..");

		const uint32_t vertexSize = m_Layout.getStride();
		const uint32_t bufferSize = vertexCount * vertexSize;
		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   Vertex: Size: {}, Count: {}", vertexSize, vertexCount);
		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   BufferSize: {}", vertexSize);

		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   Creating StagingBuffer..");
		Ref<StagingBuffer> stagingBuffer = createStagingBuffer(ctx, bufferSize);
		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:     Mapping Staging Buffer to RAM..");
		stagingBuffer->map();

		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   Creating VulkanBuffer..");
		m_VulkanBuffer = VulkanBuffer::create(
			getVulkanDevice(),
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_SHARING_MODE_EXCLUSIVE //TODO: expose
		);
		C78E_CORE_ASSERT(m_VulkanBuffer, "VulkanVertexBuffer::VulkanVertexBuffer: Failed to create VulkanBuffer!");
	}

	VulkanVertexBuffer::VulkanVertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, Ref<StagingBuffer> stagingBuffer)
		: GraphicsContextItem(ctx), VulkanGraphicsContextItem(), VertexBuffer(inputRate, layout, stagingBuffer->size() / layout.getStride()) {
		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer: Creating prestaged VulkanVertexBuffer..");
		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   StagingBuffer: Mapped to RAM: {}", (stagingBuffer->isMapped() ? "mapped" : "mapping now"));
		if (!stagingBuffer->isMapped()) {
			stagingBuffer->map();
		}
		
		const uint32_t vertexSize = m_Layout.getStride();
		const uint32_t bufferSize = stagingBuffer->size();
		const size_t vertexCount = bufferSize / vertexSize;
		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   Vertex: Size: {}, Count: {}", vertexSize, vertexCount);
		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   BufferSize: {}", bufferSize);
		
		setStagingBuffer(stagingBuffer);

		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   Creating VulkanBuffer..");
		m_VulkanBuffer = VulkanBuffer::create(
			getVulkanDevice(),
			stagingBuffer->size(),
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_SHARING_MODE_EXCLUSIVE //TODO: expose
		);
		C78E_CORE_ASSERT(m_VulkanBuffer, "VulkanVertexBuffer::VulkanVertexBuffer: Failed to create VulkanBuffer!");

		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer:   Copying StagingBuffer to VertexBuffer..");
		ctx.copyBuffer(*stagingBuffer, *this);

		C78E_CORE_TRACE("VulkanVertexBuffer::VulkanVertexBuffer: Created VulkanVertexBuffer.");
	}

	VulkanVertexBuffer::~VulkanVertexBuffer() {}

	bool VulkanVertexBuffer::alive() {
		return m_VulkanBuffer.get() != nullptr;
	}

	void VulkanVertexBuffer::free() {
		m_VulkanBuffer = nullptr;
	}

	VkVertexInputBindingDescription VulkanVertexBuffer::makeInputBindingDescription(uint32_t binding) const {
		return VkVertexInputBindingDescription{
			.binding = binding,
			.stride = m_Layout.getStride(),
			.inputRate = toVkVertexInputRate(m_VertexInputRate),
		};
	}

	std::vector<VkVertexInputAttributeDescription> VulkanVertexBuffer::makeInputAttributeDescription(uint32_t binding, uint32_t firstLayout) const {
		std::vector<VkVertexInputAttributeDescription> attributeDescription;
		attributeDescription.reserve(m_Layout.getAttributeCount());
		uint32_t layout = firstLayout;
		for (const VertexAttribute& attribute : m_Layout) {
			attributeDescription.emplace_back(
				binding,
				layout++,
				toVkFormat(attribute)
			);
		}
		return attributeDescription;
	}

}
