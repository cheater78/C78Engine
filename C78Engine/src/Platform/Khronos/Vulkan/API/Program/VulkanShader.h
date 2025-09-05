#pragma once
#include <C78E/Graphics/API/Program/Shader.h>

#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>
#include <Platform/Khronos/Vulkan/API/Program/VulkanShaderStage.h>

namespace C78E {

	VkPipelineShaderStageCreateInfo toVulkanPipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
	VkPipelineShaderStageCreateInfo toVulkanPipelineShaderStageCreateInfo(const ShaderStage& stage, Ref<Shader> shader);

	class VulkanShader : public Shader {
	public:
		VulkanShader(GraphicsContext& ctx, ShaderStage stage, Ref<ScopedBuffer> binCode);
		virtual ~VulkanShader();

	public:
		VkShaderModule getVkShaderModule() const;
	private:
		Ref<VulkanDevice> m_Device;
		VkShaderModule m_ShaderModule;
	};

}
