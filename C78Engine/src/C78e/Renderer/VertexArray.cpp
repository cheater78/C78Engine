#include "C78EPCH.h"
#include "C78E/Renderer/VertexArray.h"

#include "C78E/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace C78E {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLVertexArray>();
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}