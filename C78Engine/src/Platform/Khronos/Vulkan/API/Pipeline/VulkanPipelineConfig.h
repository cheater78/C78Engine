#pragma once
#include <C78E/Graphics/API/Pipeline/PipelineConfig.h>

namespace C78E {

	VkPrimitiveTopology toVkTopology(PrimitiveTopology topology);
	VkSampleCountFlagBits toVkSampleCount(Multisample multiSample);
	VkCompareOp toVkCompareOp(DepthFunction depthFunc);

	struct VulkanGraphicsPipelineConfig : public GraphicsPipelineConfig {
	public:
		VulkanGraphicsPipelineConfig() = default;
		virtual ~VulkanGraphicsPipelineConfig() = default;
	public:
		VkPipelineInputAssemblyStateCreateInfo getInputAssemblyInfo() const;
		VkPipelineTessellationStateCreateInfo getTessellationInfo() const;
		VkPipelineRasterizationStateCreateInfo getRasterizationInfo() const;
		VkPipelineMultisampleStateCreateInfo getMultisampleInfo() const;
		VkPipelineDepthStencilStateCreateInfo getDepthStencilInfo() const;
		VkPipelineColorBlendAttachmentState getColorBlendAttachment() const;
		VkPipelineColorBlendStateCreateInfo getColorBlendInfo(const std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachments) const;
		VkPipelineDynamicStateCreateInfo getDynamicStateInfo() const;
	protected:
		std::vector<VkDynamicState> m_DynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT, // fixed for now
			VK_DYNAMIC_STATE_SCISSOR
		};
	};

	struct VulkanComputePipelineConfig : public ComputePipelineConfig {
	public:
		virtual ~VulkanComputePipelineConfig() = default;
	public:
	};

	struct VulkanRayTracingPipelineConfig : public RayTracingPipelineConfig {
	public:
		virtual ~VulkanRayTracingPipelineConfig() = default;
	public:
	};

}
