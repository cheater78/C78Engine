#include "C78EPCH.h"
#include "RenderPass.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <C78E/Graphics/API/GraphicsContext.h>

#include <Platform/Khronos/Vulkan/API/Command/VulkanRenderPass.h>

namespace C78E {
	Ref<RenderPass> RenderPass::create(GraphicsContext& ctx) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:    return createRef<VulkanRenderPass>(ctx);
		default:
			C78E_CORE_ASSERT(false, "Unknown Graphics API!");
			return nullptr;
		}
	}
	RenderPass::RenderPass(GraphicsContext& ctx)
		: m_GraphicsContext(ctx) {
	}

}
