#pragma once
#include <C78E/Core/Buffer/DataType.h>
#include <C78E/Graphics/API/Program/ShaderDataType.h>

namespace C78E {

	using VertexAttribute = ShaderDataField;
	class VertexLayout : protected ExtListType<VertexAttribute>{
	public:
		using VertexAttributeListType = ExtListType<VertexAttribute>;
		using VertexAttributeRange = VertexAttributeListType::ListTypeRange;
		using VertexAttributeIterator = VertexAttributeRange::Iterator;
		using VertexAttributeIndex = uint32_t;
	public:
		VertexLayout()
			: VertexAttributeListType() {
		}
		template<typename... Args>
			requires (std::is_same_v<VertexAttribute, std::decay_t<Args>> && ...)
		inline VertexLayout(Args&&... args)
			: VertexAttributeListType( std::forward<Args>(args)... ) {
		}

		uint32_t getStride() const;
		uint32_t getAttributeCount() const;
		uint32_t getAttributeOffset(uint32_t index) const;

		VertexAttributeRange attributes() const;
		VertexAttributeIterator begin() const;
		VertexAttributeIterator end() const;

		VertexLayout& pushAttribute(const VertexAttribute& attribute);

	};

	using InstanceBufferLayout = VertexLayout;
	using VertexBufferLayout = VertexLayout;
	using IndexLayout = PrimitiveType;

	using UniformField = ShaderDataField; //TODO: structs in structs
	class UniformLayout : protected ExtListType<UniformField>{
	public:
		using UniformFieldListType = ExtListType<UniformField>;
		using UniformFieldRange = UniformFieldListType::ListTypeRange;
		using UniformFieldIterator = UniformFieldRange::Iterator;
		using UniformFieldIndex = uint32_t;
	public:
		UniformLayout()
			: UniformFieldListType() {
		}
		template<typename... Args>
			requires (std::is_same_v<UniformField, std::decay_t<Args>> && ...)
		inline UniformLayout(Args&&... args)
			: UniformFieldListType(std::forward<Args>(args)...) {
		}
	public:
		size_t getSize() const;
		size_t getFieldCount() const;

		UniformFieldRange fields() const;
		UniformFieldIterator begin() const;
		UniformFieldIterator end() const;

		UniformLayout& pushAttribute(const UniformField& attribute);

		bool operator==(const UniformLayout& layout) const {
			return UniformFieldListType::operator==(layout);
		}
	};

}
