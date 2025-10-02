#pragma once
#include "VulkanBufferLayout.h"
#include <Platform/Khronos/Vulkan/Core/VulkanGraphicsContextItem.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanGPUBuffer.h>
#include <C78E/Graphics/API/Buffer/VertexBuffer.h>

namespace C78E {

	VkVertexInputRate toVkVertexInputRate(VertexInputRate inputRate);

	class VulkanVertexBuffer : public VulkanGraphicsContextItem, public VulkanGPUBuffer, public VertexBuffer {
	public:
		VulkanVertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, size_t vertexCount);
		VulkanVertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, Ref<StagingBuffer> stagingBuffer);
		virtual ~VulkanVertexBuffer();

		virtual bool alive() override;
		virtual void free() override;
	public:
		VkVertexInputBindingDescription makeInputBindingDescription(uint32_t binding) const;
		std::vector<VkVertexInputAttributeDescription> makeInputAttributeDescription(uint32_t binding, uint32_t firstLayout) const;
	};

}
