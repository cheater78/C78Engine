#include "C78EPCH.h"
#include "VulkanPipelineConfig.h"

namespace C78E {

	VkPrimitiveTopology toVkTopology(PrimitiveTopology topology) {
		switch (topology) {
		case PrimitiveTopology::Points: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case PrimitiveTopology::Lines: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case PrimitiveTopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case PrimitiveTopology::Triangles: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case PrimitiveTopology::TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		case PrimitiveTopology::LinesAdjacency: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
		case PrimitiveTopology::LineStripAdjacency: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
		case PrimitiveTopology::TrianglesAdjacency: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
		case PrimitiveTopology::TriangleStripAdjacency: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
		case PrimitiveTopology::Patches: return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
		default: return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
		}
	}
	VkSampleCountFlagBits toVkSampleCount(Multisample multiSample) {
		switch (multiSample) {
		case Multisample::None: return VK_SAMPLE_COUNT_1_BIT;
		case Multisample::X2: return VK_SAMPLE_COUNT_2_BIT;
		case Multisample::X4: return VK_SAMPLE_COUNT_4_BIT;
		case Multisample::X8: return VK_SAMPLE_COUNT_8_BIT;
		case Multisample::X16: return VK_SAMPLE_COUNT_16_BIT;
		case Multisample::X32: return VK_SAMPLE_COUNT_32_BIT;
		case Multisample::X64: return VK_SAMPLE_COUNT_64_BIT;
		default: return VK_SAMPLE_COUNT_1_BIT;
		}
	}
	VkCompareOp toVkCompareOp(DepthFunction depthFunc) {
		switch (depthFunc) {
		case DepthFunction::Never: return VK_COMPARE_OP_NEVER;
		case DepthFunction::Less: return VK_COMPARE_OP_LESS;
		case DepthFunction::Equal: return VK_COMPARE_OP_EQUAL;
		case DepthFunction::LessOrEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
		case DepthFunction::Greater: return VK_COMPARE_OP_GREATER;
		case DepthFunction::NotEqual: return VK_COMPARE_OP_NOT_EQUAL;
		case DepthFunction::GreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case DepthFunction::ALWAYS: return VK_COMPARE_OP_ALWAYS;
		default: return VK_COMPARE_OP_ALWAYS;
		}
	}

	VkPipelineInputAssemblyStateCreateInfo VulkanGraphicsPipelineConfig::getInputAssemblyInfo() const {
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.pNext = nullptr;
		inputAssemblyInfo.flags = 0;
		inputAssemblyInfo.topology = toVkTopology(primitiveTopology);
		inputAssemblyInfo.primitiveRestartEnable = primitiveStripRestart ? VK_TRUE : VK_FALSE;
		return inputAssemblyInfo;
	}

	VkPipelineTessellationStateCreateInfo VulkanGraphicsPipelineConfig::getTessellationInfo() const {
		VkPipelineTessellationStateCreateInfo tessellationInfo{};
		tessellationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		tessellationInfo.pNext = nullptr;
		tessellationInfo.flags = 0;
		tessellationInfo.patchControlPoints = patchControlPoints;
		return tessellationInfo;
	}
	
	VkPipelineRasterizationStateCreateInfo VulkanGraphicsPipelineConfig::getRasterizationInfo() const {
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationInfo.pNext = nullptr;
		rasterizationInfo.flags = 0;
		rasterizationInfo.depthClampEnable = VK_FALSE;
		rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationInfo.lineWidth = lineWidth;
		rasterizationInfo.cullMode = (
			((backfaceCulling) ? VK_CULL_MODE_BACK_BIT : 0) |
			((frontFaceCulling) ? VK_CULL_MODE_FRONT_BIT : 0)
			); // CULLING
		rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizationInfo.depthBiasEnable = VK_FALSE;		// Disabled
		rasterizationInfo.depthBiasConstantFactor = 0.0f;	// Optional
		rasterizationInfo.depthBiasClamp = 0.0f;			// Optional
		rasterizationInfo.depthBiasSlopeFactor = 0.0f;		// Optional
		return rasterizationInfo;
	}

	VkPipelineMultisampleStateCreateInfo VulkanGraphicsPipelineConfig::getMultisampleInfo() const {
		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.pNext = nullptr;
		multisampleInfo.flags = 0;
		multisampleInfo.sampleShadingEnable = VK_FALSE;
		multisampleInfo.rasterizationSamples = toVkSampleCount(multisample);
		multisampleInfo.minSampleShading = 1.0f;           // Optional
		multisampleInfo.pSampleMask = nullptr;             // Optional
		multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional
		return multisampleInfo;
	}

	VkPipelineDepthStencilStateCreateInfo VulkanGraphicsPipelineConfig::getDepthStencilInfo() const {
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.pNext = nullptr;
		depthStencilInfo.flags = 0;
		depthStencilInfo.depthTestEnable = testDepthBuffer ? VK_TRUE : VK_FALSE;
		depthStencilInfo.depthWriteEnable = writeDepthBuffer ? VK_TRUE : VK_FALSE;
		depthStencilInfo.depthCompareOp = toVkCompareOp(depthFunction);
		depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		depthStencilInfo.stencilTestEnable = VK_FALSE;
		depthStencilInfo.front = {};  // Optional
		depthStencilInfo.back = {};   // Optional
		return depthStencilInfo;
	}

	VkPipelineColorBlendAttachmentState VulkanGraphicsPipelineConfig::getColorBlendAttachment() const {
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
		return colorBlendAttachment;
	}

	VkPipelineColorBlendStateCreateInfo VulkanGraphicsPipelineConfig::getColorBlendInfo(const std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachments) const {
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.pNext = nullptr;
		colorBlendInfo.flags = 0;
		colorBlendInfo.attachmentCount = colorBlendAttachments.size();
		colorBlendInfo.pAttachments = colorBlendAttachments.data();
		colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[3] = 0.0f;  // Optional
		return colorBlendInfo;
	}

	VkPipelineDynamicStateCreateInfo VulkanGraphicsPipelineConfig::getDynamicStateInfo() const {
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.pNext = nullptr;
		dynamicStateInfo.flags = 0;
		dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(m_DynamicStates.size());
		dynamicStateInfo.pDynamicStates = m_DynamicStates.data();
		return dynamicStateInfo;
	}
}
