#pragma once
#include <C78E/Graphics/API/Program/ShaderStage.h>

namespace C78E {
	
	VkShaderStageFlagBits toVkShaderStage(const ShaderStage& stage);
	VkShaderStageFlagBits toVkShaderStage(const ShaderStageBits& stages);


}
