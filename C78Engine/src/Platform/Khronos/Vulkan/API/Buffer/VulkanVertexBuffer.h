#pragma once
#include "VulkanBufferLayout.h"
#include <C78E/Graphics/API/Buffer/VertexBuffer.h>

namespace C78E {

	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, size_t vertexCount);
		VulkanVertexBuffer(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, Ref<StagingBuffer> stagingBuffer);
		virtual ~VulkanVertexBuffer();

	public:
	private:
	};

}
