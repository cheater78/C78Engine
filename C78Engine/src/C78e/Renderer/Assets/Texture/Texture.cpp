#include "C78EPCH.h"
#include "C78E/Renderer/Assets/Texture/Texture.h"

#include "C78E/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
namespace C78E {

	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification);
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(RawImage& image)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(image);
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return create(RawImage{path.c_str(), true});
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<CubeMap> CubeMap::create(std::vector<RawImage>& images) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(images);
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<CubeMap> CubeMap::create(Ref<RawImage> crossCubeMap)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(crossCubeMap);
		}

		C78_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
