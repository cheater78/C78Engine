#include "C78EPCH.h"
#include "C78E/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace C78E {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createScope<OpenGLRendererAPI>();
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}