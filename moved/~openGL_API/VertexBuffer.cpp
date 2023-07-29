/*
 * VertexBuffer.cpp
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#include "VertexBuffer.h"
#include "DebugGL.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
