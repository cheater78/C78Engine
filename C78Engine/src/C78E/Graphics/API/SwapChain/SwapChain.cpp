#include "C78EPCH.h"
#include "SwapChain.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/SwapChain/VulkanSwapChain.h>

namespace C78E {

	Ref<SwapChain> SwapChain::create(GraphicsContext& ctx, const SwapChainConfig& config) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<SwapChain>(ctx, config);
		default:
			C78E_CORE_ASSERT(false, "SwapChain::create: API not supported!");
			return nullptr;
		}
	}

	SwapChain::SwapChain(GraphicsContext& ctx, const SwapChainConfig& config)
		: m_GraphicsContext(ctx),
		m_Config(config) {
	}

	uint32_t SwapChain::frameCount() const {
		return static_cast<uint32_t>(m_Config.bufferCount);
	}

} // namespace C78E
