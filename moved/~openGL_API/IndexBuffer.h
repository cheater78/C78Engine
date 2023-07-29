/*
 * IndexBuffer.h
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_

class IndexBuffer {
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	virtual ~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return m_Count; }

private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};

#endif /* INDEXBUFFER_H_ */
