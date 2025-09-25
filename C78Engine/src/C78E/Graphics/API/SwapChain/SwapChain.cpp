#include "C78EPCH.h"
#include "SwapChain.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/SwapChain/VulkanSwapChain.h>

namespace C78E {

	Ref<SwapChain> SwapChain::create(GraphicsContext& ctx, const SwapChainConfig& config) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<VulkanSwapChain>(ctx, config);
		default:
			C78E_CORE_ASSERT(false, "SwapChain::create: API not supported!");
			return nullptr;
		}
	}

	SwapChain::SwapChain(GraphicsContext& ctx)
		: m_GraphicsContext(ctx) {
	}


} // namespace C78E
