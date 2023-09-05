#include "C78ePCH.h"
#include "C78e/Renderer/VertexArray.h"

#include "C78e/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace C78e {

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