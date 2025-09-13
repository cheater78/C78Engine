#include "C78EPCH.h"
#include "Pipeline.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipeline.h>

namespace C78E {

	Pipeline::Pipeline(GraphicsContext& ctx)
		: m_GraphicsContext(ctx) {
	}

	Ref<GraphicsPipeline> GraphicsPipeline::create(GraphicsContext& ctx, Ref<GraphicsPipelineLayout> pipelineLayout, Ref<GraphicsPipelineConfig> pipelineConfig, const GraphicsPipelineTarget& pipelineTarget) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanGraphicsPipeline>(ctx, pipelineLayout, pipelineConfig, pipelineTarget);
		default:
			C78E_CORE_ASSERT(false, "GraphicsPipeline::create: Unsupported Graphics API!");
			return nullptr;
		}
	}

	Ref<ComputePipeline> ComputePipeline::create(GraphicsContext& ctx, Ref<ComputePipelineLayout> pipelineLayout, Ref<ComputePipelineConfig> pipelineConfig) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanComputePipeline>(ctx, pipelineLayout, pipelineConfig);
		default:
			C78E_CORE_ASSERT(false, "ComputePipeline::create: Unsupported Graphics API!");
			return nullptr;
		}
	}

	Ref<RayTracingPipeline> RayTracingPipeline::create(GraphicsContext& ctx, Ref<RayTracingPipelineLayout> pipelineLayout, Ref<RayTracingPipelineConfig> pipelineConfig) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanRayTracingPipeline>(ctx, pipelineLayout, pipelineConfig);
		default:
			C78E_CORE_ASSERT(false, "GraphicsPipeline::create: Unsupported Graphics API!");
			return nullptr;
		}
	}

} // namespace C78E
