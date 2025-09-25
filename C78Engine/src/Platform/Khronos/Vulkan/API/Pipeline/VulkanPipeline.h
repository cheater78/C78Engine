#pragma once
#include <C78E/Graphics/API/Pipeline/Pipeline.h>

#include "VulkanPipelineLayout.h"
#include "VulkanPipelineConfig.h"

namespace C78E {

	class VulkanGraphicsContext;

	class VulkanPipeline : virtual public Pipeline {
	public:
		VulkanPipeline(GraphicsContext& ctx);
		virtual ~VulkanPipeline();
		
		virtual Ref<PipelineLayout> getPipelineLayout() const override = 0;
		virtual Ref<PipelineConfig> getPipelineConfig() const override = 0;
	public:
		VkPipeline getVkPipeline() const { return m_Pipeline; }
	protected:
		Ref<VulkanDevice> m_Device = nullptr;
		VkPipeline m_Pipeline;
	};

	class VulkanGraphicsPipeline : public VulkanPipeline, public GraphicsPipeline {
	public:
		VulkanGraphicsPipeline(
			GraphicsContext& ctx,
			Ref<GraphicsPipelineLayout> pipelineLayout,
			Ref<GraphicsPipelineConfig> pipelineConfig,
			const RenderArea& renderArea,
			Ref<RenderPass> renderPass,
			uint32_t subpassIndex = 0);
		~VulkanGraphicsPipeline() = default;

		virtual PipelineType getType() const override { return PipelineType::Graphics; }
		virtual Ref<PipelineLayout> getPipelineLayout() const override;
		virtual Ref<PipelineConfig> getPipelineConfig() const override;
		virtual Ref<GraphicsPipelineLayout> getGraphicsPipelineLayout() const override;
		virtual Ref<GraphicsPipelineConfig> getGraphicsPipelineConfig() const override;
		virtual Ref<VulkanGraphicsPipelineConfig> getVulkanGraphicsPipelineConfig() const;
		virtual Ref<VulkanGraphicsPipelineLayout> getVulkanComputePipelineLayout() const;
	protected:
		Ref<VulkanGraphicsPipelineConfig> m_PipelineConfig;
		Ref<VulkanGraphicsPipelineLayout> m_PipelineLayout;
	};

	class VulkanComputePipeline : public VulkanPipeline, public ComputePipeline {
	public:
		VulkanComputePipeline(GraphicsContext& ctx, Ref<ComputePipelineLayout> pipelineLayout, Ref<ComputePipelineConfig> pipelineConfig);
		~VulkanComputePipeline() = default;

		virtual PipelineType getType() const override { return PipelineType::Compute; }
		virtual Ref<PipelineConfig> getPipelineConfig() const override;
		virtual Ref<PipelineLayout> getPipelineLayout() const override;
		virtual Ref<ComputePipelineConfig> getComputePipelineConfig() const override;
		virtual Ref<ComputePipelineLayout> getComputePipelineLayout() const override;
		virtual Ref<VulkanComputePipelineConfig> getVulkanComputePipelineConfig() const;
		virtual Ref<VulkanComputePipelineLayout> getVulkanComputePipelineLayout() const;
	protected:
		Ref<VulkanComputePipelineConfig> m_PipelineConfig;
		Ref<VulkanComputePipelineLayout> m_PipelineLayout;
	};

	class VulkanRayTracingPipeline : public VulkanPipeline, public RayTracingPipeline {
	public:
		VulkanRayTracingPipeline(GraphicsContext& ctx, Ref<RayTracingPipelineLayout> pipelineLayout, Ref<RayTracingPipelineConfig> pipelineConfig);
		~VulkanRayTracingPipeline() = default;

		virtual PipelineType getType() const override { return PipelineType::RayTracing; }
		virtual Ref<PipelineLayout> getPipelineLayout() const override;
		virtual Ref<PipelineConfig> getPipelineConfig() const override;
		virtual Ref<RayTracingPipelineConfig> getRayTracingPipelineConfig() const override;
		virtual Ref<RayTracingPipelineLayout> getRayTracingPipelineLayout() const override;
		virtual Ref<VulkanRayTracingPipelineConfig> getVulkanRayTracingPipelineConfig() const;
		virtual Ref<VulkanRayTracingPipelineLayout> getVulkanRayTracingPipelineLayout() const;
	protected:
		Ref<VulkanRayTracingPipelineConfig> m_PipelineConfig;
		Ref<VulkanRayTracingPipelineLayout> m_PipelineLayout;
	};

}
