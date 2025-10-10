#pragma once
#include "BufferLayout.h"
#include "GPUBuffer.h"
#include "StagingBuffer.h"

namespace C78E {

	class UniformBuffer : public virtual StagedBuffer {
	public:
		static Ref<UniformBuffer> create(GraphicsContext& ctx, const UniformLayout& layout);
		static Ref<UniformBuffer> create(GraphicsContext& ctx, const UniformLayout& layout, Ref<StagingBuffer> stagingBuffer);
	public:
		UniformBuffer(const UniformLayout& layout);
		virtual ~UniformBuffer();

		virtual bool alive() = 0;
		virtual void free() = 0;
	public:
		const UniformLayout getUniformLayout() const;

		template<typename T>
		T& mapAs() {
			if (!m_StagingBuffer) {
				m_StagingBuffer = StagingBuffer::create(m_GraphicsContext, m_Layout.getSize());
			}
			C78E_CORE_ASSERT(m_StagingBuffer->size() == m_Layout.getSize(), "UniformBuffer::mapAs: StagingBuffer was not the size specified in m_Layout!");
			m_StagingBuffer->map();

			T* const ubo = reinterpret_cast<T*>(m_StagingBuffer->data());
			C78E_CORE_ASSERT(ubo, "UniformBuffer::mapAs: StagingBuffer could not be cast!");
			return *ubo;
		}
	protected:
		UniformLayout m_Layout;
	};

}
