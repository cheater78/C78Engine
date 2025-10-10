#include "C78EPCH.h"
#include "Texture.h"
#include "C78E/Core/Image/Image.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Image/VulkanTexture.h>

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

	Ref<Texture2D> Texture2D::create(Ref<Image2D> image, const Texture2D::Option& option) {
		switch (GraphicsInstance::api()) {
		case API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		//case API::Vulkan:  return createRef<VulkanTexture2D>(image, option);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}
	Ref<Texture2D> Texture2D::create(const ScopedBuffer& textureData, const Texture2D::Specification& specification, const Texture2D::Option& option) {
		switch (GraphicsInstance::api()) {
		case API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			//case API::OpenGL:  return createRef<VulkanTexture2D>(textureData, specification, option);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}
	Ref<Texture2D> Texture2D::create(TextureHandle rendererID, const Texture2D::Specification& specification) {
		switch (GraphicsInstance::api()) {
		case API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			//case API::OpenGL:  return createRef<VulkanTexture2D>(rendererID, specification);
		default: C78E_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

	Texture2D::Texture2D() {
		m_Specification = createRef<Texture2D::Specification>();
		m_Option = createRef<Texture2D::Option>();
	}

	const Texture2D::Specification& Texture2D::getSpecification() const { return Texture::getSpecification<Texture2D::Specification>(); }

	Texture2D::Option& Texture2D::getOption() { return Texture::getOption<Texture2D::Option>(); }

	const Texture2D::Option& Texture2D::getOption() const { return Texture::getOption<Texture2D::Option>(); }

	uint32_t Texture2D::getWidth() const { return Texture2D::getSpecification().width; }

	uint32_t Texture2D::getHeight() const { return Texture2D::getSpecification().height; }

	Texture2D::Specification& Texture2D::getSpecification() { return Texture::getSpecification<Texture2D::Specification>(); }
	

	// CubeMap

	CubeMap::Specification::Specification(Ref<Image2D> image)
		: Texture::Specification(), size(image->getSize().x) {
		format = image->getFormat();
	}

	CubeMap::CubeImageData::CubeImageData() {
		const static uint32_t rgba = 0xFFFF;
		Ref<Image2D> white = Image2D::create(ImageFormat::RGBA8, ImageSize(1,1), &rgba);
		for(Ref<Image2D>& face : *this) {
			face = white;
		}
	}

	CubeMap::CubeImageData::CubeImageData(Ref<Image2D> top, Ref<Image2D> bot, Ref<Image2D> front, Ref<Image2D> back, Ref<Image2D> left, Ref<Image2D> right) {
		topImage() = top;
		bottomImage() = bot;
		frontImage() = front;
		backImage() = back;
		leftImage() = left;
		rightImage() = right;
	}

	CubeMap::Specification CubeMap::CubeImageData::toSpecification() const {
		return CubeMap::Specification((*this)[(Face)0]);
	}

	Ref<Image2D> CubeMap::CubeImageData::operator[](Face face) {
		return std::array<Ref<Image2D>, 6>::operator[]((uint8_t)face);
	}

	Ref<Image2D> CubeMap::CubeImageData::operator[](Face face) const {
		return std::array<Ref<Image2D>, 6>::operator[]((uint8_t)face);
	}

	CubeMap::CubeMap() {
		m_Specification = createRef<CubeMap::Specification>();
		m_Option = createRef<CubeMap::Option>();
	}

	Ref<CubeMap> CubeMap::create(const CubeImageData& cubeMapImageData, const CubeMap::Option& option) {
		switch (GraphicsInstance::api()) {
		case API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			//case API::OpenGL:  return createRef<OpenGLCubeMap>(cubeMapImageData, option);
		default:
			C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		}
		return nullptr;
	}

	Ref<CubeMap> CubeMap::create(Ref<Image2D> crossCubeMap, const CubeMap::Option& option) {
		switch (GraphicsInstance::api()) {
		case API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			//case API::OpenGL:  return createRef<OpenGLCubeMap>(crossCubeMap, option);
		default:
			C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		}
		return nullptr;
	}

	Ref<CubeMap> CubeMap::create(const ScopedBuffer& textureData, const CubeMap::Specification& specification, const CubeMap::Option& option) {
		switch (GraphicsInstance::api()) {
		case API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			//case API::OpenGL:  return createRef<OpenGLCubeMap>(textureData, specification, option);
		default:
			C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		}
		return nullptr;
	}

	Ref<CubeMap> CubeMap::create(TextureHandle rendererID, const CubeMap::Specification& specification) {
		switch (GraphicsInstance::api()) {
		case API::None:    C78E_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			//case API::OpenGL:  return createRef<OpenGLCubeMap>(rendererID, specification);
		default:
			C78E_CORE_ASSERT(false, "Unknown RendererAPI!");
		}
		return nullptr;
	}

	const CubeMap::Specification& CubeMap::getSpecification() const {
		return Texture::getSpecification<CubeMap::Specification>();
	}

	CubeMap::Option& CubeMap::getOption() {
		return Texture::getOption<CubeMap::Option>();
	}

	const CubeMap::Option& CubeMap::getOption() const {
		return Texture::getOption<CubeMap::Option>();
	}

	uint32_t CubeMap::getSize() const {
		return CubeMap::getSpecification().size;
	}

	CubeMap::Specification& CubeMap::getSpecification() {
		return Texture::getSpecification<CubeMap::Specification>();
	}
	
}
