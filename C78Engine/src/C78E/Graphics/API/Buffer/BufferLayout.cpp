#include "C78EPCH.h"
#include "BufferLayout.h"

namespace C78E {

	VertexAttribute::VertexAttribute()
		: ShaderDataType(PrimitiveType::None, 0), m_Name("Unnamed") {
	}

	VertexAttribute::VertexAttribute(const std::string& name, PrimitiveType::Type type, size_t count)
		: ShaderDataType(type, count), m_Name(name) {
	}

	uint32_t VertexLayout::getStride() const {
		return static_cast<uint32_t>(size());
	}

	uint32_t VertexLayout::getAttributeCount() const {
		return static_cast<uint32_t>(elementCount());
	}

	uint32_t VertexLayout::getAttributeOffset(uint32_t index) const {
		return static_cast<uint32_t>(elementAlignment(static_cast<ListType::Index>(index)));
	}

	VertexLayout::AttributeRange VertexLayout::attributes() {
		return AttributeListType::elements();
	}

	const VertexLayout::AttributeRange VertexLayout::attributes() const {
		return AttributeListType::elements();
	}

	VertexLayout::AttributeIterator VertexLayout::begin() {
		return AttributeListType::begin();
	}

	VertexLayout::AttributeIterator VertexLayout::end() {
		return AttributeListType::end();
	}

	VertexLayout::AttributeIterator VertexLayout::begin() const {
		return AttributeListType::begin();
	}

	VertexLayout::AttributeIterator VertexLayout::end() const {
		return AttributeListType::end();
	}

	VertexLayout& VertexLayout::pushAttribute(const Attribute& attribute) {
		pushField(attribute);
		return *this;
	}

}
