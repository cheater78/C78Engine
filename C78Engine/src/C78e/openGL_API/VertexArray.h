/*
 * VertexArray.h
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#ifndef VERTEXARRAY_H_
#define VERTEXARRAY_H_

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
public:
	VertexArray();
	virtual ~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;

private:
	unsigned int m_RendererID;
};

#endif /* VERTEXARRAY_H_ */
