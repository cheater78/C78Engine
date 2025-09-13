#include "C78EPCH.h"
#include "ShaderStage.h"

namespace C78E { 
	
	ShaderStage ShaderStage::parseShaderStageFromSourceString(std::string_view shaderStage) {
		if(shaderStage == "vertex")
			return ShaderStage(ShaderStage::Vertex);
		if(shaderStage == "tessellation")
			return ShaderStage(ShaderStage::TessellationControl);
		if(shaderStage == "tessellationEvaluation")
			return ShaderStage(ShaderStage::TessellationEvaluation);
		if(shaderStage == "geometry")
			return ShaderStage(ShaderStage::Geometry);
		if(shaderStage == "fragment" || shaderStage == "pixel")
			return ShaderStage(ShaderStage::Fragment);
		if(shaderStage == "compute")
			return ShaderStage(ShaderStage::Compute);
		C78E_CORE_FATAL("ShaderShaderStage::parseShaderShaderStageFromSourceString: string '{}' does not represent a ShaderShaderStage!", shaderStage);
		return ShaderStage(None);
	}

	ShaderStage ShaderStage::shaderStageFromString(const std::string& shaderStage) {
		if(shaderStage == "ShaderShaderStage::None")
			return ShaderStage(ShaderStage::None);
		if(shaderStage == "ShaderShaderStage::Vertex")
			return ShaderStage(ShaderStage::Vertex);
		if(shaderStage == "ShaderShaderStage::Tesselation")
			return ShaderStage(ShaderStage::TessellationControl);
		if(shaderStage == "ShaderShaderStage::TessEvaluation")
			return ShaderStage(ShaderStage::TessellationEvaluation);
		if(shaderStage == "ShaderShaderStage::Geometry")
			return ShaderStage(ShaderStage::Geometry);
		if(shaderStage == "ShaderShaderStage::Fragment" || shaderStage == "ShaderShaderStage::Pixel")
			return ShaderStage(ShaderStage::Fragment);
		if(shaderStage == "ShaderShaderStage::Compute")
			return ShaderStage(ShaderStage::Compute);
		C78E_CORE_FATAL("ShaderShaderStage::shaderShaderStageFromString: string '{}' does not represent a ShaderShaderStage!", shaderStage);
		return ShaderStage(ShaderStage::None);
	}

	std::string ShaderStage::shaderStageToString(ShaderStage ShaderStage) {
		switch(ShaderStage.stage()) {
		case ShaderStage::None:					return "ShaderShaderStage::None";
		case ShaderStage::Vertex:					return "ShaderShaderStage::Vertex";
		case ShaderStage::TessellationControl:			return "ShaderShaderStage::Tesselation";
		case ShaderStage::TessellationEvaluation:	return "ShaderShaderStage::TessEvaluation";
		case ShaderStage::Geometry:				return "ShaderShaderStage::Geometry";
		case ShaderStage::Fragment:				return "ShaderShaderStage::Fragment";
		case ShaderStage::Compute:				return "ShaderShaderStage::Compute";
		default:
			C78E_CORE_FATAL("ShaderCompiler::shaderShaderStageToString: ShaderShaderStage was unknown!");
		}
		return "ShaderShaderStage::None";
	}

	std::string_view ShaderStage::shaderStageExtensionFragment(ShaderStage stage) {
		switch (stage.stage()) {
			case ShaderStage::Vertex:
				return C78E_FILE_EXT_FRAG_SHADER_VERTEX;
			case ShaderStage::TessellationControl:
				return C78E_FILE_EXT_FRAG_SHADER_TESSELATION;
			case ShaderStage::TessellationEvaluation:
				return C78E_FILE_EXT_FRAG_SHADER_TESSEVALUATION;
			case ShaderStage::Geometry:
				return C78E_FILE_EXT_FRAG_SHADER_GEOMETRY;
			case ShaderStage::Fragment:
				return C78E_FILE_EXT_FRAG_SHADER_FRAGMENT;
			case ShaderStage::Compute:
				return C78E_FILE_EXT_FRAG_SHADER_COMPUTE;
		default:
			C78E_CORE_FATAL("ShaderStage::shaderStageExtensionFragment: ShaderShaderStage was unknown!");
			return std::string_view();
		}
	}

	ShaderStage::ShaderStage(Type type) : m_Type(type) {
	}

	inline const ShaderStage::Type& ShaderStage::stage() const {
		return m_Type;
	}

}
