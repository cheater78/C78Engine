#include "C78EPCH.h"
#include "UniformBuffer.h"

#include "C78E/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace C78E {

	Ref<UniformBuffer> UniformBuffer::create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLUniformBuffer>(size);
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLUniformBuffer>(size, binding);
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
