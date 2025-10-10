#include "C78EPCH.h"
#include "BufferLayout.h"

namespace C78E {

	uint32_t VertexLayout::getStride() const {
		return static_cast<uint32_t>(size());
	}

	uint32_t VertexLayout::getAttributeCount() const {
		return static_cast<uint32_t>(elementCount());
	}

	uint32_t VertexLayout::getAttributeOffset(uint32_t index) const {
		return static_cast<uint32_t>(elementAlignment(static_cast<ListType::Index>(index)));
	}

	VertexLayout::VertexAttributeRange VertexLayout::attributes() const {
		return VertexAttributeListType::elements();
	}

	VertexLayout::VertexAttributeIterator VertexLayout::begin() const {
		return VertexAttributeListType::begin();
	}

	VertexLayout::VertexAttributeIterator VertexLayout::end() const {
		return VertexAttributeListType::end();
	}

	VertexLayout& VertexLayout::pushAttribute(const VertexAttribute& attribute) {
		pushField(attribute);
		return *this;
	}


	size_t UniformLayout::getSize() const {
		return UniformFieldListType::alignedSize();
	}

	size_t UniformLayout::getFieldCount() const {
		return UniformFieldListType::elementCount();
	}

	UniformLayout::UniformFieldRange UniformLayout::fields() const {
		return UniformFieldListType::elements();
	}

	UniformLayout::UniformFieldIterator UniformLayout::begin() const {
		return UniformFieldListType::begin();
	}

	UniformLayout::UniformFieldIterator UniformLayout::end() const {
		return UniformFieldListType::end();
	}

	UniformLayout& UniformLayout::pushAttribute(const UniformField& attribute) {
		pushField(attribute);
		return *this;
	}

	
}
