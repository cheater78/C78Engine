#pragma once
#include "VulkanBufferLayout.h"
#include "VulkanGPUBuffer.h"
#include <Platform/Khronos/Vulkan/Core/VulkanGraphicsContextItem.h>
#include <C78E/Graphics/API/Buffer/UniformBuffer.h>

namespace C78E {

	class VulkanUniformBuffer : public VulkanGraphicsContextItem, public VulkanGPUBuffer, public UniformBuffer {
	public:
		VulkanUniformBuffer(GraphicsContext& ctx, const UniformLayout& layout);
		VulkanUniformBuffer(GraphicsContext& ctx, const UniformLayout& layout, Ref<StagingBuffer> stagingBuffer);
		virtual ~VulkanUniformBuffer();

		virtual bool alive() override { return false; }
		virtual void free() override {}
	protected:

	};

}
