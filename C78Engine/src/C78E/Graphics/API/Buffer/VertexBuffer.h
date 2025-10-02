#pragma once
#include "GPUBuffer.h"
#include "StagingBuffer.h"
#include "BufferLayout.h"

namespace C78E {

	enum class VertexInputRate {
		Vertex,
		Instance
	};

	class VertexBuffer : public virtual GPUBuffer, public StagedBuffer {
	public:
		static Ref<VertexBuffer> create(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, size_t vertexCount);
		static Ref<VertexBuffer> create(GraphicsContext& ctx, VertexInputRate inputRate, const VertexBufferLayout& layout, Ref<StagingBuffer> stagingBuffer);
	public:
		VertexBuffer(VertexInputRate inputRate, const VertexBufferLayout& layout, size_t vertexCount);
		virtual ~VertexBuffer() = default;

	public:
		VertexInputRate getVertexInputRate() const;
		VertexBufferLayout getVertexBufferLayout() const;
		size_t getVertexCount() const;
	protected:
		VertexInputRate m_VertexInputRate;
		VertexBufferLayout m_Layout;
		size_t m_VertexCount;
	};

}
