/*
 * VertexBuffer.h
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

class VertexBuffer {
public:
	VertexBuffer(const void* data, unsigned int size);
	virtual ~VertexBuffer();

	void bind() const;
	void unbind() const;

private:
	unsigned int m_RendererID;
};

#endif /* VERTEXBUFFER_H_ */
