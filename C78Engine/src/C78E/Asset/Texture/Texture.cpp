#include "C78EPCH.h"
#include "Texture.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <Platform/OpenGL/OpenGLTexture.h>

namespace C78E {

	std::string Texture::Type::textureTypeToString(Texture::Type type) {
		switch ((uint8_t)type) {
		case Texture::Type::None: return "Texture::Type::None";
		case Texture::Type::Flat: return "Texture::Type::Flat";
		case Texture::Type::Sphere: return "Texture::Type::Sphere";
		default: C78E_CORE_ASSERT(false, "Texture::Type::textureTypeToString: illegal Type!"); return "Texture::Type::None";
		}
	}

	Texture::Type Texture::Type::textureTypeFromString(const std::string& typeString) {
		if(typeString == "Texture::Type::None") return Texture::Type::None;
		if(typeString == "Texture::Type::Flat") return Texture::Type::Flat;
		if(typeString == "Texture::Type::Sphere") return Texture::Type::Sphere;
		C78E_CORE_ASSERT(false, "Texture::Type::textureTypeFromString: illegal typeString!"); return 0;
	}

	std::string Texture::Filter::filterToString(Filter type) {
		switch ((uint8_t)type) {
		case Texture::Filter::Linear: return "Texture::Filter::Linear";
		case Texture::Filter::Nearest: return "Texture::Filter::Nearest";
		case Texture::Filter::MipMap: return "Texture::Filter::MipMap";
		default: C78E_CORE_ASSERT(false, "Texture::Filter::filterToString: illegal Type!"); return "Texture::Filter::Linear";
		}
	}

	Texture::Filter Texture::Filter::filterFromString(const std::string& filterString) {
		if (filterString == "Texture::Filter::Linear") return Texture::Filter::Linear;
		if (filterString == "Texture::Filter::Nearest") return Texture::Filter::Nearest;
		if (filterString == "Texture::Filter::MipMap") return Texture::Filter::MipMap;
		C78E_CORE_ASSERT(false, "Texture::Filter::textureTypeFromString: illegal filterString!"); return 0;
	}

	std::string Texture::Wrap::wrapToString(Wrap wrap) {
		switch ((uint8_t)wrap) {
		case Texture::Wrap::Repeat: return "Texture::Wrap::Repeat";
		case Texture::Wrap::MirroredRepeat: return "Texture::Wrap::MirroredRepeat";
		case Texture::Wrap::ClampToEdge: return "Texture::Wrap::ClampToEdge";
		case Texture::Wrap::ClampToBorder: return "Texture::Wrap::ClampToBorder";
		default: C78E_CORE_ASSERT(false, "Texture::Wrap::wrapToString: illegal wrap Mode!"); return "Texture::Wrap::None";
		}
	}

	Texture::Wrap Texture::Wrap::wrapFromString(const std::string& wrapString) {
		if (wrapString == "Texture::Wrap::Repeat") return Texture::Wrap::Repeat;
		if (wrapString == "Texture::Wrap::MirroredRepeat") return Texture::Wrap::MirroredRepeat;
		if (wrapString == "Texture::Wrap::ClampToEdge") return Texture::Wrap::ClampToEdge;
		if (wrapString == "Texture::Wrap::ClampToBorder") return Texture::Wrap::ClampToBorder;
		C78E_CORE_ASSERT(false, "Texture::Wrap::wrapFromString: illegal wrapString!"); return 0;
	}

	// Texture2D

	Texture2D::Texture2D() {
		m_Specification = createRef<Texture2D::Specification>();
		m_Option = createRef<Texture2D::Option>();
	}

	Ref<Texture2D> Texture2D::create(const Image& image, const Texture2D::Option& option) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(image, option);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::create(const ScopedBuffer& textureData, const Texture2D::Specification& specification, const Texture2D::Option& option) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(textureData, specification, option);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::create(TextureHandle rendererID, const Texture2D::Specification& specification) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(rendererID, specification);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	const Texture2D::Specification& Texture2D::getSpecification() const { return Texture::getSpecification<Texture2D::Specification>(); }

	Texture2D::Option& Texture2D::getOption() { return Texture::getOption<Texture2D::Option>(); }

	const Texture2D::Option& Texture2D::getOption() const { return Texture::getOption<Texture2D::Option>(); }

	uint32_t Texture2D::getWidth() const { return Texture2D::getSpecification().width; }

	uint32_t Texture2D::getHeight() const { return Texture2D::getSpecification().height; }

	Texture2D::Specification& Texture2D::getSpecification() { return Texture::getSpecification<Texture2D::Specification>(); }
	

	// CubeMap

	CubeMap::Specification::Specification(const Image& image)
		: Texture::Specification(), size(image.getWidth()) {
		format = image.getFormat();
	}

	CubeMap::CubeImageData::CubeImageData()
		: std::array<Image, 6>{} {
	}

	CubeMap::CubeImageData::CubeImageData(std::initializer_list<Image> init) {
		std::copy(init.begin(), init.end(), this->begin());
	}

	CubeMap::Specification CubeMap::CubeImageData::toSpecification() const {
		return CubeMap::Specification((*this)[(Face)0]);
	}

	Image& CubeMap::CubeImageData::operator[](Face face) {
		return std::array<Image, 6>::operator[]((uint8_t)face);
	}

	const Image& CubeMap::CubeImageData::operator[](Face face) const {
		return std::array<Image, 6>::operator[]((uint8_t)face);
	}

	CubeMap::CubeMap() {
		m_Specification = createRef<CubeMap::Specification>();
		m_Option = createRef<CubeMap::Option>();
	}

	Ref<CubeMap> CubeMap::create(const CubeImageData& cubeMapImageData, const CubeMap::Option& option) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(cubeMapImageData, option);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<CubeMap> CubeMap::create(const Image& crossCubeMap, const CubeMap::Option& option) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(crossCubeMap, option);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<CubeMap> CubeMap::create(const ScopedBuffer& textureData, const CubeMap::Specification& specification, const CubeMap::Option& option) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(textureData, specification, option);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<CubeMap> CubeMap::create(TextureHandle rendererID, const CubeMap::Specification& specification) {
		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLCubeMap>(rendererID, specification);
		}
		C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	const CubeMap::Specification& CubeMap::getSpecification() const { return Texture::getSpecification<CubeMap::Specification>(); }

	CubeMap::Option& CubeMap::getOption() { return Texture::getOption<CubeMap::Option>(); }

	const CubeMap::Option& CubeMap::getOption() const { return Texture::getOption<CubeMap::Option>(); }

	uint32_t CubeMap::getSize() const { return CubeMap::getSpecification().size; }

	CubeMap::Specification& CubeMap::getSpecification() { return Texture::getSpecification<CubeMap::Specification>(); }
	
}
