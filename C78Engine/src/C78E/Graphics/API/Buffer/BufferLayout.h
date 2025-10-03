#pragma once
#include <C78E/Core/Buffer/DataType.h>
#include <C78E/Graphics/API/Program/ShaderDataType.h>

namespace C78E {

	class VertexAttribute : public ShaderDataType {
	public:
		VertexAttribute();
		VertexAttribute(const std::string& name, PrimitiveType::Type type, size_t count = 1);
		~VertexAttribute() = default;

	protected:
		std::string m_Name;
	};

	class VertexLayout : protected ExtListType<VertexAttribute>{
	public:
		using Attribute = VertexAttribute;
		using AttributeListType = ExtListType<Attribute>;
		using AttributeRange = AttributeListType::ListTypeRange;
		using AttributeIterator = AttributeRange::Iterator;
		using AttributeIndex = uint32_t;
	public:
		VertexLayout()
			: AttributeListType() {
		}
		template<typename... Args>
			requires (std::is_same_v<Attribute, std::decay_t<Args>> && ...)
		inline VertexLayout(Args&&... args)
			: AttributeListType( std::forward<Args>(args)... ) {
		}

		uint32_t getStride() const;
		uint32_t getAttributeCount() const;
		uint32_t getAttributeOffset(uint32_t index) const;

		AttributeRange attributes() const;
		AttributeIterator begin() const;
		AttributeIterator end() const;

		VertexLayout& pushAttribute(const Attribute& attribute);

	};
	using InstanceBufferLayout = VertexLayout;
	using VertexBufferLayout = VertexLayout;

	using IndexLayout = PrimitiveType;

	class UniformLayout {

	};

}
