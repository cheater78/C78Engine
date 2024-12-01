#include "C78EPCH.h"
#include "Texture.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLTexture.h>

namespace C78E {

	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification, const Buffer& data) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification, data);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification, uint32_t rendererID) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification, rendererID);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::create(const Image& image) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(image);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<CubeMap> CubeMap::create(std::vector<Image>& images) {
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(images);
			default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<CubeMap> CubeMap::create(Ref<Image> crossCubeMap) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(crossCubeMap);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

}
