#include "C78EPCH.h"
#include "C78E/Renderer/GraphicsContext.h"

#include "C78E/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace C78E {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}