#include "C78EPCH.h"
#include "GraphicsContext.h"

#include <C78E/Graphics/Window/Window.h>
#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>

namespace C78E {

	Scope<GraphicsContext> GraphicsContext::create(Window& window) {
		switch(GraphicsInstance::api()) {
		case API::Vulkan:  return createScope<VulkanGraphicsContext>(window);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	GraphicsContext::GraphicsContext(Window& window)
		: m_Window(window) {
	}

}
