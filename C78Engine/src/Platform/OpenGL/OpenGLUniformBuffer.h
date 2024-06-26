#pragma once

#include <C78E/Renderer/API/UniformBuffer.h>

namespace C78E {

	class OpenGLUniformBuffer : public UniformBuffer {
	public:
		OpenGLUniformBuffer(uint32_t size);
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) override;
		virtual void bind(uint32_t binding) override;

	private:
		uint32_t m_RendererID = 0;
	};
}
