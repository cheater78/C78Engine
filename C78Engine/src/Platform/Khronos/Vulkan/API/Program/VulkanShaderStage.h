#pragma once
#include <C78E/Graphics/API/Program/ShaderStage.h>

namespace C78E {
	
	VkShaderStageFlagBits toVkShaderStage(const ShaderStage& stage);
	VkShaderStageFlags toVkShaderStage(ShaderStages stages);


}
