#include "C78EPCH.h"
#include "VulkanShader.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>

namespace C78E {

	VkPipelineShaderStageCreateInfo toVulkanPipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule) {
		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = stage;
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = "main";
		shaderStageInfo.pSpecializationInfo = nullptr;
		return shaderStageInfo;
	}

	VkPipelineShaderStageCreateInfo toVulkanPipelineShaderStageCreateInfo(const ShaderStage& stage, Ref<Shader> shader) {
		VkShaderStageFlagBits vulkanStage = toVkShaderStage(stage);
		Ref<VulkanShader> vulkanShader = castRef<VulkanShader>(shader);
		C78E_CORE_ASSERT(vulkanShader, "toVulkanPipelineShaderStageCreateInfo: Shader was not a valid VulkanShader!");
		return toVulkanPipelineShaderStageCreateInfo(vulkanStage, vulkanShader->getVkShaderModule());
	}

	VulkanShader::VulkanShader(GraphicsContext& ctx, ShaderStage stage, Ref<ScopedBuffer> binCode)
		: Shader(ctx, stage) {

		VulkanGraphicsContext& vulkanContext = ctx.getAs<VulkanGraphicsContext>();
		m_Device = vulkanContext.getDevice();

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = binCode->size();
		createInfo.pCode = binCode->as<uint32_t>();

		VkResult result = vkCreateShaderModule(m_Device->getVkDevice(), &createInfo, nullptr, &m_ShaderModule);
		C78E_CORE_ASSERT(result == VK_SUCCESS, "VulkanShader::VulkanShader: Failed to create shader module!");

	}

	VulkanShader::~VulkanShader() {
		if(m_ShaderModule != nullptr) {
			vkDestroyShaderModule(m_Device->getVkDevice(), m_ShaderModule, nullptr);
			m_ShaderModule = VK_NULL_HANDLE;
		}
	}

	VkShaderModule VulkanShader::getVkShaderModule() const {
		return m_ShaderModule;
	}

}
