#include "C78EPCH.h"
#include "GraphicsContext.h"

#include <C78E/Core/Window/Window.h>
#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>

namespace C78E {

	Scope<GraphicsContext> GraphicsContext::create(Window& window) {
		switch(GraphicsInstance::api()) {
		case API::Vulkan:  return createScope<VulkanGraphicsContext>(window);
		default:
			C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		}
		return nullptr;
	}

	GraphicsContext::GraphicsContext(Window& window)
		: m_Window(window) {
	}

	bool GraphicsContext::hasSwapChain() const {
		return m_SwapChain != nullptr;
	}

	SwapChain& GraphicsContext::createSwapChain(const SwapChainConfig& swapChainConfig) {
		m_SwapChain = SwapChain::create(*this, swapChainConfig);
		return *m_SwapChain;
	}

	SwapChain& GraphicsContext::getSwapChain() const {
		return *m_SwapChain;
	}

	// Shader Manager
	Ref<ShaderManager> GraphicsContext::createShaderManager(const FilePath& cacheDirectory) {
		C78E_CORE_TRACE("GraphicsContext::createSwapChain: creating ShaderManager...")
			return m_ShaderManager = createRef<ShaderManager>(*this, cacheDirectory);
	}

	Ref<ShaderManager> GraphicsContext::getShaderManager() const {
		return m_ShaderManager;
	}

}
