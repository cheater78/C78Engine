#include "C78EPCH.h"
#include "ShaderDataType.h"

namespace C78E {

	ShaderDataField::ShaderDataField()
		: VectorType(PrimitiveType::None, 0), m_Name("Unnamed ShaderDataField") {
	}

	ShaderDataField::ShaderDataField(const std::string& name, PrimitiveType::Type type, size_t count)
		: VectorType(type, count), m_Name(name) {
	}

	const std::string& ShaderDataField::getFieldName() const {
		return m_Name;
	}

}
