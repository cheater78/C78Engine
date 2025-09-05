#include "C78EPCH.h"
#include "PipelineLayout.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipelineLayout.h>

namespace C78E {

	Ref<PipelineLayout> PipelineLayout::create(PipelineType type) {
		switch (type) {
			case PipelineType::Graphics:
				return GraphicsPipelineLayout::create();
			case PipelineType::Compute:
				return ComputePipelineLayout::create();
			case PipelineType::RayTracing:
				return RayTracingPipelineLayout::create();
			default:
				C78E_CORE_ASSERT(false, "PipelineLayout::create: Unsupported PipelineType!");
				return nullptr;
		}
	}




}
