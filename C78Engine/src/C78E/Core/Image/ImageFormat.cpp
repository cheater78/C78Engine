#include "C78EPCH.h"
#include "ImageFormat.h"

namespace C78E {

	std::string ImageFormat::imageFormatToString(ImageFormat imageFormat) {
		switch(imageFormat.m_ImagePixelLayout) {
		case None:		return "ImageFormat::None";
		case R8:		return "ImageFormat::R8";
		case R16F:		return "ImageFormat::R16F";
		case R32:		return "ImageFormat::R32";
		case R32F:		return "ImageFormat::R32F";
		case D24S8:		return "ImageFormat::D24S8";
		case RG8:		return "ImageFormat::RG8";
		case RG16F:		return "ImageFormat::RG16F";
		case RG32F:		return "ImageFormat::RG32F";
		case RGB8:		return "ImageFormat::RGB8";
		case RGB16F:	return "ImageFormat::RGB16F";
		case RGB32F:	return "ImageFormat::RGB32F";
		case RGBA8:		return "ImageFormat::RGBA8";
		case RGBA16F:	return "ImageFormat::RGBA16F";
		case RGBA32F:	return "ImageFormat::RGBA32F";
		case ABGR8:		return "ImageFormat::ABGR8";
		default:		return "ImageFormat::None";
		}
	}

	ImageFormat ImageFormat::imageFormatFromString(const std::string& imageFormatString) {
		if(imageFormatString == "ImageFormat::None") return None;
		if(imageFormatString == "ImageFormat::R8") return R8;
		if(imageFormatString == "ImageFormat::R16F") return R16F;
		if(imageFormatString == "ImageFormat::R32") return R32;
		if(imageFormatString == "ImageFormat::R32F") return R32F;
		if(imageFormatString == "ImageFormat::D24S8") return D24S8;
		if(imageFormatString == "ImageFormat::RG8") return RG8;
		if(imageFormatString == "ImageFormat::RG16F") return RG16F;
		if(imageFormatString == "ImageFormat::RG32F") return RG32F;
		if(imageFormatString == "ImageFormat::RGB8") return RGB8;
		if(imageFormatString == "ImageFormat::RGB16F") return RGB16F;
		if(imageFormatString == "ImageFormat::RGB32F") return RGB32F;
		if(imageFormatString == "ImageFormat::RGBA8") return RGBA8;
		if(imageFormatString == "ImageFormat::RGBA16F") return RGBA16F;
		if(imageFormatString == "ImageFormat::RGBA32F") return RGBA32F;
		if(imageFormatString == "ImageFormat::ABGR8") return ABGR8;
		return None;
	}

	size_t ImageFormat::imageFormatByteSize(ImagePixelLayout imagePixelLayout) {
		switch(imagePixelLayout) {
		case None: return 0;
		case R8: return 1;
		case R16F: return 2;
		case R32: return 4;
		case R32F: return 4;
		case D24S8: return 4;
		case RG8: return 2;
		case RG16F: return 4;
		case RG32F: return 8;
		case RGB8: return 3;
		case RGB16F: return 6;
		case RGB32F: return 12;
		case RGBA8: return 4;
		case RGBA16F: return 8;
		case RGBA32F: return 16;
		case ABGR8: return 16;
		default: return 0;
		}
	}

	size_t ImageFormat::imageFormatElementCount(ImagePixelLayout imagePixelLayout) {
		switch(imagePixelLayout) {
		case None: return 0;
		case R8: return 1;
		case R16F: return 1;
		case R32: return 1;
		case R32F: return 1;
		case D24S8: return 2;
		case RG8: return 2;
		case RG16F: return 2;
		case RG32F: return 2;
		case RGB8: return 3;
		case RGB16F: return 3;
		case RGB32F: return 3;
		case RGBA8: return 4;
		case RGBA16F: return 4;
		case RGBA32F: return 4;
		case ABGR8: return 4;
		default: return 0;
		}
	}

	size_t ImageFormat::imageFormatBitsPerElement(ImagePixelLayout imagePixelLayout) {
		switch(imagePixelLayout) {
		case None: return 0;
		case R8: return 8;
		case R16F: return 16;
		case R32: return 32;
		case R32F: return 32;
		case D24S8: return 32;
		case RG8: return 16;
		case RG16F: return 32;
		case RG32F: return 64;
		case RGB8: return 24;
		case RGB16F: return 48;
		case RGB32F: return 96;
		case RGBA8: return 32;
		case RGBA16F: return 64;
		case RGBA32F: return 128;
		case ABGR8: return 32;
		default: return 0;
		}
	}

	bool ImageFormat::isDepthFormat(ImageFormat imageFormat) {
		return imageFormat == ImagePixelLayout::D24S8;
	}

	bool ImageFormat::isColorFormat(ImageFormat imageFormat) {
		return !isDepthFormat(imageFormat);
	}

	bool ImageFormat::isSDRFormat(ImageFormat imageFormat) {
		return imageFormat == ImagePixelLayout::R8 ||
			imageFormat == ImagePixelLayout::RG8 ||
			imageFormat == ImagePixelLayout::RGB8 ||
			imageFormat == ImagePixelLayout::RGBA8 ||
			imageFormat == ImagePixelLayout::ABGR8 ||
			false; // just so the line above can be duplicated easily
	}

	bool ImageFormat::isHDRFormat(ImageFormat imageFormat) {
		return imageFormat == ImagePixelLayout::R16F ||
			imageFormat == ImagePixelLayout::R32F ||
			imageFormat == ImagePixelLayout::RG16F ||
			imageFormat == ImagePixelLayout::RG32F ||
			imageFormat == ImagePixelLayout::RGB16F ||
			imageFormat == ImagePixelLayout::RGB32F ||
			imageFormat == ImagePixelLayout::RGBA16F ||
			imageFormat == ImagePixelLayout::RGBA32F ||
			false; // just so the line above can be duplicated easily
	}

	ImageFormat::ImageFormat() : m_ImagePixelLayout(None) {
	}

	ImageFormat::ImageFormat(ImagePixelLayout imagePixelLayout)
		: m_ImagePixelLayout(imagePixelLayout) {
	}

	ImageFormat::~ImageFormat() {
	}

	size_t ImageFormat::getElementCount() const {
		return imageFormatElementCount(m_ImagePixelLayout);
	}

	size_t ImageFormat::getBitsPerElement() const {
		return imageFormatBitsPerElement(m_ImagePixelLayout);
	}

	size_t ImageFormat::getByteSize() const {
		return imageFormatByteSize(m_ImagePixelLayout);
	}

	ImageFormat::ImagePixelLayout ImageFormat::format() const {
		return m_ImagePixelLayout;
	}

	bool ImageFormat::operator==(const ImageFormat& other) const {
		return format() == other.format();
	}

	bool ImageFormat::operator!=(const ImageFormat& other) const {
		return !operator==(other);
	}

}
