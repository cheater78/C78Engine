#include "C78EPCH.h"
#include "OpenGLUniformBuffer.h"

#include <glad.h>

namespace C78E {


	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size) {
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_STATIC_DRAW); 
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_STATIC_DRAW); // TODO: investigate usage hint
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}


	void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset) {
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}


	void OpenGLUniformBuffer::bind(uint32_t binding) {
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}
}
