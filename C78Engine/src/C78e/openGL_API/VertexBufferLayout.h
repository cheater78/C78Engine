/*
 * VertexBufferLayout.h
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#ifndef VERTEXBUFFERLAYOUT_H_
#define VERTEXBUFFERLAYOUT_H_

#include <vector>
#include "DebugGL.h"

struct VertexBufferLayoutElement {
	unsigned int type;
	unsigned int count;
	unsigned char normailzed;

	static unsigned int getSizeOfType(unsigned int type){
		switch(type){
		case(GL_FLOAT): 			return 4;
		case(GL_UNSIGNED_INT): 		return 4;
		case(GL_UNSIGNED_BYTE): 	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {
public:
	VertexBufferLayout() : m_Stride(0) { }
	//~VertexBufferLayout();

	template<typename T> inline void push(unsigned int count);


	inline const std::vector<VertexBufferLayoutElement> getElements() const& { return m_Elements; }
	inline unsigned int getStride() const { return m_Stride; }

private:
	std::vector<VertexBufferLayoutElement> m_Elements;
	unsigned int m_Stride;
};

template<typename T> inline void VertexBufferLayout::push(unsigned int count){
		//static_assert(false);
	}

template<> inline void VertexBufferLayout::push<float>(unsigned int count){
	m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
	m_Stride += VertexBufferLayoutElement::getSizeOfType(GL_FLOAT) * count;
}

template<> inline void VertexBufferLayout::push<unsigned int>(unsigned int count){
	m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
	m_Stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_INT) * count;
}

template<> inline void VertexBufferLayout::push<unsigned char>(unsigned int count){
	m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
	m_Stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
}



#endif /* VERTEXBUFFERLAYOUT_H_ */
