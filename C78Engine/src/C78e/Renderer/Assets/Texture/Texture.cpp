#include "C78EPCH.h"
#include "C78E/Renderer/Assets/Texture/Texture.h"

#include "C78E/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace C78E {

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification);
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(path);
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
