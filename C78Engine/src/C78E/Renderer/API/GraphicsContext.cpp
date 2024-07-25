#include "C78EPCH.h"
#include "GraphicsContext.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLGraphicsContext.h>

namespace C78E {

	Scope<GraphicsContext> GraphicsContext::create(void* window) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createScope<OpenGLGraphicsContext>(static_cast<GLFWwindow*>(window));
		}
		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}