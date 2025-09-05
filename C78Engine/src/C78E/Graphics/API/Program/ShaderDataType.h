#pragma once
#include <C78E/Core/Buffer/DataType.h>

namespace C78E {

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

}
