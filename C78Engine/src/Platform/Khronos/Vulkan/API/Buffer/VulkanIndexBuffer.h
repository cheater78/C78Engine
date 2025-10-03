#pragma once
#include "VulkanBufferLayout.h"
#include <Platform/Khronos/Vulkan/Core/VulkanGraphicsContextItem.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanGPUBuffer.h>
#include <C78E/Graphics/API/Buffer/IndexBuffer.h>

namespace C78E {

	VkIndexType toVkIndexType(const IndexLayout& layout);

	class VulkanIndexBuffer : public VulkanGraphicsContextItem, public VulkanGPUBuffer, public IndexBuffer {
	public:
		VulkanIndexBuffer(GraphicsContext& ctx, const IndexLayout& layout, size_t indexCount);
		VulkanIndexBuffer(GraphicsContext& ctx, const IndexLayout& layout, Ref<StagingBuffer> stagingBuffer);
		virtual ~VulkanIndexBuffer();

		virtual bool alive() override { return false; }
		virtual void free() override { }
	protected:

	};

}
