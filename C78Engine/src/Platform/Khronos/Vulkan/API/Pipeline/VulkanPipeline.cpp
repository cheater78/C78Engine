#include "C78E/Graphics/API/Pipeline/Pipeline.h"
#include "C78EPCH.h"
#include "VulkanPipeline.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipelineLayout.h>
#include <Platform/Khronos/Vulkan/API/Image/VulkanFrameBuffer.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanRenderPass.h>

namespace C78E {

	VulkanPipeline::VulkanPipeline(GraphicsContext& ctx)
		: Pipeline(ctx), m_Device(ctx.getAs<VulkanGraphicsContext>().getDevice()) {
	}

	VulkanPipeline::~VulkanPipeline() {
		if (m_Pipeline != VK_NULL_HANDLE) {
			vkDestroyPipeline(m_Device->getVkDevice(), m_Pipeline, nullptr);
		}
	}

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(
		GraphicsContext& ctx,
		Ref<GraphicsPipelineLayout> pipelineLayout,
		Ref<GraphicsPipelineConfig> pipelineConfig,
		const RenderArea& renderArea,
		Ref<RenderPass> renderPass,
		uint32_t subpassIndex)
		: Pipeline(ctx), VulkanPipeline(ctx),
		m_PipelineLayout(castRef<VulkanGraphicsPipelineLayout>(pipelineLayout)),
		m_PipelineConfig(castRef<VulkanGraphicsPipelineConfig>(pipelineConfig)) {
		C78E_CORE_ASSERT(m_PipelineLayout, "VulkanPipeline: PipelineLayout was not a valid VulkanGraphicsPipelineLayout!");
		C78E_CORE_ASSERT(m_PipelineConfig, "VulkanPipeline: PipelineConfig was not a valid VulkanGraphicsPipelineConfig!");

		//TODO: create the actual VkPipelineLayout - not pretty
		m_PipelineLayout->init();


		const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages = m_PipelineLayout->getVkPipelineShaderStageCreateInfos();

		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{};
		// Pipeline type
		graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphicsPipelineCreateInfo.pNext = VK_NULL_HANDLE;
		graphicsPipelineCreateInfo.flags = 0;

		// Shader Stages
		const uint32_t shaderStageCount = static_cast<uint32_t>(shaderStages.size());
		graphicsPipelineCreateInfo.stageCount = shaderStageCount;
		graphicsPipelineCreateInfo.pStages = (shaderStageCount) ? shaderStages.data() : VK_NULL_HANDLE;

		// Graphics Pipeline Layout - need to exist during pipeline creation
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = m_PipelineLayout->getVertexInputInfo();
		graphicsPipelineCreateInfo.pVertexInputState = &vertexInputInfo;

		// Graphics Pipeline Config - need to exist during pipeline creation
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = m_PipelineConfig->getInputAssemblyInfo();
		graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyInfo;
		VkPipelineTessellationStateCreateInfo tessellationInfo = m_PipelineConfig->getTessellationInfo();
		graphicsPipelineCreateInfo.pTessellationState = &tessellationInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationState = m_PipelineConfig->getRasterizationInfo();
		graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
		VkPipelineMultisampleStateCreateInfo multisampleState = m_PipelineConfig->getMultisampleInfo();
		graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
		VkPipelineDepthStencilStateCreateInfo depthStencilState = m_PipelineConfig->getDepthStencilInfo(); // Potential issue if no depth/stencil
		graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilState;

		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments = { m_PipelineConfig->getColorBlendAttachment() };
		VkPipelineColorBlendStateCreateInfo colorBlendInfo = m_PipelineConfig->getColorBlendInfo(colorBlendAttachments);
		graphicsPipelineCreateInfo.pColorBlendState = &colorBlendInfo;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo = m_PipelineConfig->getDynamicStateInfo();
		graphicsPipelineCreateInfo.pDynamicState = &dynamicStateInfo;

		// Graphics Pipeline Target - need to exist during pipeline creation
		VkViewport viewport{};
		viewport.x = static_cast<float>(renderArea.viewport.offset.x);
		viewport.y = static_cast<float>(renderArea.viewport.offset.y);
		viewport.width = static_cast<float>(renderArea.viewport.size.x);
		viewport.height = static_cast<float>(renderArea.viewport.size.y);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { static_cast<int32_t>(renderArea.scissor.offset.x), static_cast<int32_t>(renderArea.scissor.offset.y) };
		scissor.extent = { static_cast<uint32_t>(renderArea.scissor.size.x), static_cast<uint32_t>(renderArea.scissor.size.y) };

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.pNext = nullptr;
		viewportInfo.flags = 0;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = &viewport;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = &scissor;
		graphicsPipelineCreateInfo.pViewportState = &viewportInfo;

		// Pipeline Layout
		graphicsPipelineCreateInfo.layout = m_PipelineLayout->getVkPipelineLayout();

		// Render Pass
		Ref<VulkanRenderPass> vulkanRenderPass = castRef<VulkanRenderPass>(renderPass);
		graphicsPipelineCreateInfo.renderPass = vulkanRenderPass->getVkRenderPass();
		graphicsPipelineCreateInfo.subpass = subpassIndex;

		graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		graphicsPipelineCreateInfo.basePipelineIndex = -1;

		VkResult result = vkCreateGraphicsPipelines(
			m_Device->getVkDevice(), //Device
			VK_NULL_HANDLE, //PipelineCache
			1, // CreateInfoCount
			&graphicsPipelineCreateInfo, // CreateInfo
			nullptr, // AllocationCallbacks
			&m_Pipeline // Pipeline
		);
		C78E_CORE_SOFT_VALIDATE(result == VK_SUCCESS, "VulkanPipeline: Failed to create GraphicsPipeline!");
	}

	Ref<PipelineLayout> VulkanGraphicsPipeline::getPipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<PipelineConfig> VulkanGraphicsPipeline::getPipelineConfig() const {
		return m_PipelineConfig;
	}
	Ref<GraphicsPipelineLayout> VulkanGraphicsPipeline::getGraphicsPipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<GraphicsPipelineConfig> VulkanGraphicsPipeline::getGraphicsPipelineConfig() const {
		return m_PipelineConfig;
	}
	Ref<VulkanGraphicsPipelineLayout> VulkanGraphicsPipeline::getVulkanComputePipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<VulkanGraphicsPipelineConfig> VulkanGraphicsPipeline::getVulkanGraphicsPipelineConfig() const {
		return m_PipelineConfig;
	}

	VulkanComputePipeline::VulkanComputePipeline(GraphicsContext& ctx, Ref<ComputePipelineLayout> pipelineLayout, Ref<ComputePipelineConfig> pipelineConfig)
		: Pipeline(ctx), VulkanPipeline(ctx),
		m_PipelineLayout(castRef<VulkanComputePipelineLayout>(pipelineLayout)),
		m_PipelineConfig(castRef<VulkanComputePipelineConfig>(pipelineConfig)) {

		Ref<VulkanPipelineLayout> vulkanPipelineLayout = castRef<VulkanPipelineLayout>(pipelineLayout);
		C78E_CORE_ASSERT(vulkanPipelineLayout, "toVkComputePipelineCreateInfo: PipelineLayout was not a valid VulkanPipelineLayout!");
		C78E_CORE_ASSERT(vulkanPipelineLayout->getType() == PipelineType::Compute, "toVkComputePipelineCreateInfo: PipelineLayout was not a valid Compute PipelineLayout!");
		const VkPipelineShaderStageCreateInfo shaderStage = vulkanPipelineLayout->getVkPipelineShaderStageCreateInfos().front();

		VkComputePipelineCreateInfo computePipelineCreateInfo{};
		computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		computePipelineCreateInfo.pNext = VK_NULL_HANDLE;
		computePipelineCreateInfo.flags = 0;

		// Shader Stages
		computePipelineCreateInfo.stage = shaderStage; // Assuming only one stage for compute

		// Pipeline Layout
		computePipelineCreateInfo.layout = vulkanPipelineLayout->getVkPipelineLayout();

		computePipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		computePipelineCreateInfo.basePipelineIndex = -1;

		VkResult result = vkCreateComputePipelines(
			m_Device->getVkDevice(), //Device
			VK_NULL_HANDLE, //PipelineCache
			1, // CreateInfoCount
			&computePipelineCreateInfo, // CreateInfo
			nullptr, // AllocationCallbacks
			&m_Pipeline // Pipeline
		);
		C78E_CORE_SOFT_VALIDATE(result == VK_SUCCESS, "VulkanPipeline: Failed to create ComputePipeline!");

	}

	Ref<PipelineLayout> VulkanComputePipeline::getPipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<PipelineConfig> VulkanComputePipeline::getPipelineConfig() const {
		return m_PipelineConfig;
	}
	Ref<ComputePipelineLayout> VulkanComputePipeline::getComputePipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<ComputePipelineConfig> VulkanComputePipeline::getComputePipelineConfig() const {
		return m_PipelineConfig;
	}
	Ref<VulkanComputePipelineLayout> VulkanComputePipeline::getVulkanComputePipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<VulkanComputePipelineConfig> VulkanComputePipeline::getVulkanComputePipelineConfig() const {
		return m_PipelineConfig;
	}

	VulkanRayTracingPipeline::VulkanRayTracingPipeline(GraphicsContext& ctx, Ref<RayTracingPipelineLayout> pipelineLayout, Ref<RayTracingPipelineConfig> pipelineConfig)
		: Pipeline(ctx), VulkanPipeline(ctx),
		m_PipelineLayout(castRef<VulkanRayTracingPipelineLayout>(pipelineLayout)),
		m_PipelineConfig(castRef<VulkanRayTracingPipelineConfig>(pipelineConfig)) {

		Ref<VulkanPipelineLayout> vulkanPipelineLayout = castRef<VulkanPipelineLayout>(pipelineLayout);
		C78E_CORE_ASSERT(vulkanPipelineLayout, "toVkRayTracingPipelineCreateInfo: PipelineLayout was not a valid VulkanPipelineLayout!");
		C78E_CORE_ASSERT(vulkanPipelineLayout->getType() == PipelineType::RayTracing, "toVkRayTracingPipelineCreateInfo: PipelineLayout was not a valid Ray Tracing PipelineLayout!");
		const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages = vulkanPipelineLayout->getVkPipelineShaderStageCreateInfos();

		VkRayTracingPipelineCreateInfoKHR rayTracingPipelineCreateInfo{};

		rayTracingPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
		rayTracingPipelineCreateInfo.pNext = VK_NULL_HANDLE;
		rayTracingPipelineCreateInfo.flags = 0;

		// Shader Stages
		const uint32_t shaderStageCount = static_cast<uint32_t>(shaderStages.size());
		rayTracingPipelineCreateInfo.stageCount = shaderStageCount;
		rayTracingPipelineCreateInfo.pStages = (shaderStageCount) ? shaderStages.data() : VK_NULL_HANDLE;

		// Ray Tracing Groups
		rayTracingPipelineCreateInfo.groupCount = 0; // Set this if you have ray tracing groups
		rayTracingPipelineCreateInfo.pGroups = nullptr; // Set this if you have ray tracing groups
		rayTracingPipelineCreateInfo.maxPipelineRayRecursionDepth = 1; // Set this to your desired recursion depth

		// Pipeline Layout
		rayTracingPipelineCreateInfo.layout = vulkanPipelineLayout->getVkPipelineLayout();

		rayTracingPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		rayTracingPipelineCreateInfo.basePipelineIndex = -1;
		/*
		VkResult result = vkCreateRayTracingPipelinesKHR(
			m_Device->getVkDevice(), //Device
			deferedOperation, // Deferred Operation
			VK_NULL_HANDLE, //PipelineCache
			1, // CreateInfoCount
			&rayTracingPipelineCreateInfo, // CreateInfo
			nullptr, // AllocationCallbacks
			&m_Pipeline // Pipeline
		);
		C78E_CORE_SOFT_VALIDATE(result == VK_SUCCESS, "VulkanPipeline: Failed to create Ray Tracing Pipeline!");
		*/

	}

	Ref<PipelineLayout> VulkanRayTracingPipeline::getPipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<PipelineConfig> VulkanRayTracingPipeline::getPipelineConfig() const {
		return m_PipelineConfig;
	}
	Ref<RayTracingPipelineLayout> VulkanRayTracingPipeline::getRayTracingPipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<RayTracingPipelineConfig> VulkanRayTracingPipeline::getRayTracingPipelineConfig() const {
		return m_PipelineConfig;
	}
	Ref<VulkanRayTracingPipelineLayout> VulkanRayTracingPipeline::getVulkanRayTracingPipelineLayout() const {
		return m_PipelineLayout;
	}
	Ref<VulkanRayTracingPipelineConfig> VulkanRayTracingPipeline::getVulkanRayTracingPipelineConfig() const {
		return m_PipelineConfig;
	}

}
