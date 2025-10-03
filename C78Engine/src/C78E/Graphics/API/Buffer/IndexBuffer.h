#pragma once
#include "GPUBuffer.h"
#include "StagingBuffer.h"
#include "BufferLayout.h"

namespace C78E {

	class IndexBuffer : public virtual GPUBuffer, public StagedBuffer {
	public:
		static Ref<IndexBuffer> create(GraphicsContext& ctx, const IndexLayout& layout, size_t indexCount);
		static Ref<IndexBuffer> create(GraphicsContext& ctx, const IndexLayout& layout, Ref<StagingBuffer> stagingBuffer);
	public:
		IndexBuffer(const IndexLayout& layout, size_t indexCount);
		virtual ~IndexBuffer();

	public:
		IndexLayout getIndexLayout() const;
		size_t getIndexCount() const;
	protected:
		IndexLayout m_Layout;
		size_t m_IndexCount;
	};

}
