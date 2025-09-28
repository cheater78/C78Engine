#pragma once
#include <C78E/Core/Buffer/DataType.h>

#include <C78E/Graphics/API/Program/ShaderDataType.h>

namespace C78E {

	class VertexAttribute : public ShaderDataType {
	public:
		VertexAttribute()
			: ShaderDataType(PrimitiveType::None, 0), m_Name("Unnamed") {
		}
		VertexAttribute(const std::string& name, PrimitiveType::Type type, size_t count = 1)
			: ShaderDataType(type, count), m_Name(name) {
		}

	protected:
		std::string m_Name;
	};

	class VertexLayout : protected ExtListType<VertexAttribute>{
	public:
		using Attribute = VertexAttribute;
		using AttributeListType = ExtListType<Attribute>;
		using AttributeRange = MemoryRange<Attribute>;
		using AttributeIterator = AttributeRange::Iterator;
		using AttributeIndex = uint32_t;
	public:
		template<typename... Args>
		VertexLayout(Args&&... args)
			: ExtListType<VertexAttribute>( std::forward<Args>(args)... ) {
		}

		uint32_t getStride() const {
			return static_cast<uint32_t>(size());
		}
		uint32_t getAttributeCount() const {
			return static_cast<uint32_t>(elementCount());
		}
		uint32_t getAttributeOffset(uint32_t index) const {
			return static_cast<uint32_t>(elementAlignment(static_cast<ListType::Index>(index)));
		}

		AttributeRange attributes() {
			return AttributeListType::elements();
		}
		const AttributeRange attributes() const {
			return AttributeListType::elements();
		}
		AttributeIterator begin() {
			return AttributeListType::begin();
		}
		AttributeIterator end() {
			return AttributeListType::end();
		}
		const AttributeIterator begin() const {
			return AttributeListType::begin();
		}
		const AttributeIterator end() const{
			return AttributeListType::end();
		}

		VertexLayout& pushAttribute(const Attribute& attribute) {
			pushField(attribute);
			return *this;
		}

	};
	using InstanceBufferLayout = VertexLayout;
	using VertexBufferLayout = VertexLayout;

	class UniformLayout {

	};

}
