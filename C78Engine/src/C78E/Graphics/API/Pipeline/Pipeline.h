#pragma once
#include "PipelineType.h"
#include "PipelineLayout.h"
#include "PipelineConfig.h"

#include <C78E/Graphics/API/Buffer/FrameBuffer.h>

namespace C78E {

	class GraphicsContext;

	class Pipeline {
	public:
		//static Ref<Pipeline> create(Ref<PipelineLayout> pipelineLayout, Ref<PipelineConfig> pipelineConfig);
	public:
		Pipeline(GraphicsContext& ctx);
		virtual ~Pipeline() = default;

		virtual PipelineType getType() const = 0;
		virtual Ref<PipelineLayout> getPipelineLayout() const = 0;
		virtual Ref<PipelineConfig> getPipelineConfig() const = 0;
	protected:
		GraphicsContext& m_GraphicsContext;
	};

	struct GraphicsPipelineTarget {
		Ref<RenderPass> renderPass;
		uint32_t subpassIndex = 0;
		ImagePosition renderAreaOffset = ImagePosition(0, 0);
		ImageSize renderAreaSize = ImageSize(0, 0); // if 0,0 uses full framebuffer size
		ImagePosition scissorOffset = ImagePosition(0, 0);
		ImageSize scissorSize = ImageSize(0, 0); // if 0,0 uses full framebuffer size
	};

	class GraphicsPipeline : public virtual Pipeline {
	public:
		static Ref<GraphicsPipeline> create(GraphicsContext& ctx, Ref<GraphicsPipelineLayout> pipelineLayout, Ref<GraphicsPipelineConfig> pipelineConfig, const GraphicsPipelineTarget& pipelineTarget);
	public:
		virtual ~GraphicsPipeline() = default;
		virtual PipelineType getType() const override final { return PipelineType::Graphics; }

		virtual Ref<PipelineLayout> getPipelineLayout() const override = 0;
		virtual Ref<PipelineConfig> getPipelineConfig() const override = 0;
		virtual Ref<GraphicsPipelineLayout> getGraphicsPipelineLayout() const = 0;
		virtual Ref<GraphicsPipelineConfig> getGraphicsPipelineConfig() const = 0;
	};

	class ComputePipeline : public virtual Pipeline {
	public:
		static Ref<ComputePipeline> create(GraphicsContext& ctx, Ref<ComputePipelineLayout> pipelineLayout, Ref<ComputePipelineConfig> pipelineConfig);
	public:
		virtual ~ComputePipeline() = default;
		virtual PipelineType getType() const override final { return PipelineType::Compute; }

		virtual Ref<PipelineLayout> getPipelineLayout() const override = 0;
		virtual Ref<PipelineConfig> getPipelineConfig() const override = 0;
		virtual Ref<ComputePipelineLayout> getComputePipelineLayout() const = 0;
		virtual Ref<ComputePipelineConfig> getComputePipelineConfig() const = 0;
	};

	class RayTracingPipeline : public virtual Pipeline {
	public:
		static Ref<RayTracingPipeline> create(GraphicsContext& ctx, Ref<RayTracingPipelineLayout> pipelineLayout, Ref<RayTracingPipelineConfig> pipelineConfig);
	public:
		virtual ~RayTracingPipeline() = default;
		virtual PipelineType getType() const override final { return PipelineType::RayTracing; }

		virtual Ref<PipelineLayout> getPipelineLayout() const override = 0;
		virtual Ref<PipelineConfig> getPipelineConfig() const override = 0;
		virtual Ref<RayTracingPipelineLayout> getRayTracingPipelineLayout() const = 0;
		virtual Ref<RayTracingPipelineConfig> getRayTracingPipelineConfig() const = 0;
	};
	
}
