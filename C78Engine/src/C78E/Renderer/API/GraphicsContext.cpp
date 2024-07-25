#include "C78EPCH.h"
#include "GraphicsContext.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLGraphicsContext.h>
#include <Platform/Vulkan/VulkanGraphicsContext.h>

namespace C78E {

	Scope<GraphicsContext> GraphicsContext::create(Window& window) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createScope<OpenGLGraphicsContext>(window);
			case RendererAPI::API::Vulkan:  return createScope<VulkanGraphicsContext>(window);
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}