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
		return ShaderStage(Vertex);
	}

	ShaderStage ShaderStage::shaderStageFromString(const std::string& shaderStage) {
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
		return ShaderStage(ShaderStage::Vertex);
	}

	std::string ShaderStage::shaderStageToString(ShaderStage ShaderStage) {
		switch(ShaderStage.stage()) {
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

	const ShaderStage::Type& ShaderStage::stage() const {
		return m_Type;
	}

	bool ShaderStage::operator==(const ShaderStage& other) const {
		return other.stage() == stage();
	}

	ShaderStage ShaderStages::toShaderStage(ShaderStages stages) {
		const ShaderStageBitsType bits = stages.m_Bits;
		const ShaderStageBitsType stageIndex = std::countr_zero(bits);
		switch (stageIndex) {
		case ShaderStage::Vertex: return ShaderStage::Vertex;
		case ShaderStage::TessellationControl: return ShaderStage::TessellationControl;
		case ShaderStage::TessellationEvaluation: return ShaderStage::TessellationEvaluation;
		case ShaderStage::Geometry: return ShaderStage::Geometry;
		case ShaderStage::Fragment: return ShaderStage::Fragment;
		case ShaderStage::Compute: return ShaderStage::Compute;
		case ShaderStage::RayGeneration: return ShaderStage::RayGeneration;
		case ShaderStage::Intersection: return ShaderStage::Intersection;
		case ShaderStage::AnyHit: return ShaderStage::AnyHit;
		case ShaderStage::ClosestHit: return ShaderStage::ClosestHit;
		case ShaderStage::Miss: return ShaderStage::Miss;
		case ShaderStage::Callable: return ShaderStage::Callable;
		default:
			C78E_CORE_ASSERT(false, "ShaderStages::toShaderStage: stages did not contain a valid ShaderStage!");
		}
		return ShaderStage::Vertex;
	}

	ShaderStages::ShaderStages(ShaderStageBitsType bits)
		: m_Bits(bits) {
	}

	ShaderStages::ShaderStageBitsType ShaderStages::raw() const {
		return m_Bits;
	}

	ShaderStages::StageRange ShaderStages::stages() const {
		return StageRange(m_Bits);
	}

	ShaderStages::StageIterator ShaderStages::begin() const {
		return stages().begin();
	}

	ShaderStages::StageIterator ShaderStages::end() const {
		return stages().end();
	}

	ShaderStages::ShaderStageBitsType ShaderStages::size() const {
		return stages().size();
	}

	bool ShaderStages::operator==(const ShaderStages& other) const {
		return other.m_Bits == m_Bits;
	}

	bool ShaderStages::operator&=(const ShaderStages& other) {
		return m_Bits &= other.m_Bits;
	}

	ShaderStages::operator bool() const {
		return m_Bits;
	}


	ShaderStages operator&(const ShaderStages& a, const ShaderStages& b) {
		return a.raw() & b.raw();
	}
	ShaderStages operator|(const ShaderStages& a, const ShaderStages& b) {
		return a.raw() | b.raw();
	}
}
