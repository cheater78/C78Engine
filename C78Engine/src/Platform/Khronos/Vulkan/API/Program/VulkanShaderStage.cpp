#include "C78EPCH.h"
#include "VulkanShaderStage.h"

namespace C78E {

	VkShaderStageFlagBits toVkShaderStage(const ShaderStage& stage) {
		switch(stage.stage()) {
		case ShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		case ShaderStage::TessellationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case ShaderStage::TessellationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case ShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
		case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		default: C78E_CORE_ASSERT(false, "Unknown ShaderStage!");
		}
		return (VkShaderStageFlagBits)0;
	}

	VkShaderStageFlagBits toVkShaderStage(const ShaderStageBits& stages) {
		return (VkShaderStageFlagBits)stages;
	}

} // namespace C78E
