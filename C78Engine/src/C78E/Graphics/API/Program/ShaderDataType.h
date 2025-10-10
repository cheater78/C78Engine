#pragma once
#include <C78E/Core/Buffer/DataType.h>

namespace C78E {

	// What Shader Data Types are there? -> ShaderDataType
	// - glsl primitives -> (name <-> static VectorType)
	// - arrays -> (name, Vec<ShaderDataType>)
	// - structs -> (name, ExtListType<ShaderDataType>)
	// ...?
	class ShaderDataType : public VectorType {
	public:
		ShaderDataType() = default;
		ShaderDataType(PrimitiveType::Type type, size_t count = 1)
			: VectorType(type, count) {
		}
		ShaderDataType(const VectorType& type) 
			: VectorType(type) {
		}
		ShaderDataType(const ShaderDataType&) = default;
		ShaderDataType(ShaderDataType&&) = default;
		ShaderDataType& operator=(const ShaderDataType&) = default;
		virtual ~ShaderDataType() = default;


		ShaderDataType& operator=(const VectorType& other) {
			VectorType::operator=(other);
			return *this;
		}
	};

	//TODO: for complete ShaderReflection:

	// What Shader Data Fields are there? -> ShaderDataField
	// - Vertex/Instance Attribute (name, location, VectorType(limited))
	// - UniformBufferObject (name, binding, VectorType(limited))
	// - TextureSampler (name, binding) TODO: 2D,...?
	// ...?
	class ShaderDataField : public VectorType {
	public:
		ShaderDataField();
		ShaderDataField(const std::string& name, PrimitiveType::Type type, size_t count = 1);
		~ShaderDataField() = default;

	public:
		const std::string& getFieldName() const;
	protected:
		std::string m_Name;
	};
	
}
