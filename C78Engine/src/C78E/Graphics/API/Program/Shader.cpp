#include "C78EPCH.h"
#include "Shader.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <C78E/Graphics/API/GraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Program/VulkanShader.h>

namespace C78E {

	Ref<Shader> Shader::create(GraphicsContext& ctx, ShaderStage stage, Ref<ScopedBuffer> binCode) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanShader>(ctx, stage, binCode);
		default:
			C78E_CORE_ASSERT(false, "Shader::create: Unsupported Graphics API");
			return nullptr;
		}
	}

	Shader::Shader(GraphicsContext& ctx, ShaderStage stage)
		: m_GraphicsContext(ctx), m_Stage(stage) {
	}

}
