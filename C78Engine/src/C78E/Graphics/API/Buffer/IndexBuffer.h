#pragma once
#include "GPUBuffer.h"
#include "StagingBuffer.h"
#include "BufferLayout.h"

namespace C78E {

	class IndexBuffer : public virtual StagedBuffer {
	public:
		static Ref<IndexBuffer> create(GraphicsContext& ctx, const IndexLayout& layout, size_t indexCount);
		static Ref<IndexBuffer> create(GraphicsContext& ctx, const IndexLayout& layout, Ref<StagingBuffer> stagingBuffer);
	public:
		IndexBuffer(const IndexLayout& layout, size_t indexCount);
		virtual ~IndexBuffer();

		virtual bool alive() = 0;
		virtual void free() = 0;
	public:
		IndexLayout getIndexLayout() const;
		size_t getIndexCount() const;
	protected:
		IndexLayout m_Layout;
		size_t m_IndexCount;
	};

}
