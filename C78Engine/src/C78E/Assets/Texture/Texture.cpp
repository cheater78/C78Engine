#include "C78EPCH.h"
#include "C78E/Assets/Texture/Texture.h"

#include "C78E/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
namespace C78E {

	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification);
		default: C78_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification, const Buffer& data) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification, data);
		default: C78_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification, uint32_t rendererID) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification, rendererID);
		default: C78_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::create(Image& image) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(image);
		default: C78_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<CubeMap> CubeMap::create(std::vector<Image>& images) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(images);
			default: C78_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<CubeMap> CubeMap::create(Ref<Image> crossCubeMap) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    C78_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(crossCubeMap);
		default: C78_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

}
