#include <C78EPCH.h>
#include "Image.h"

namespace C78E {

	const size_t Image::ImageFormat::imageFormatSize(const ImageFormat& imageFormat) {
		if ((uint8_t)imageFormat.m_Format <= 4) return (uint8_t)imageFormat.m_Format;
		else if (imageFormat.m_Format == ImageFormat::RGBA32F) return 16;
		else if (imageFormat.m_Format == ImageFormat::R32) return 4;
		else if (imageFormat.m_Format == ImageFormat::D24S8) return 4;
		else C78E_CORE_ERROR("Image::ImageFormat::imageFormatSize: illegal ImageFormat!"); return 0;
	}

	std::string Image::ImageFormat::imageFormatToString(const ImageFormat& imageFormat) {
		switch (imageFormat.m_Format) {
		case C78E::Image::ImageFormat::None: return "ImageFormat::None";
		case C78E::Image::ImageFormat::R8: return "ImageFormat::R8";
		case C78E::Image::ImageFormat::RG8: return "ImageFormat::RG8";
		case C78E::Image::ImageFormat::RGB8: return "ImageFormat::RGB8";
		case C78E::Image::ImageFormat::RGBA8: return "ImageFormat::RGBA8";
		case C78E::Image::ImageFormat::RGBA32F: return "ImageFormat::RGBA32F";
		case C78E::Image::ImageFormat::R32: return "ImageFormat::R32";
		case C78E::Image::ImageFormat::D24S8: return "ImageFormat::D24S8";
		default: return "ImageFormat::None";
		}
	}

	Image::ImageFormat Image::ImageFormat::imageFormatFromString(const std::string& str) {
		if (str == "ImageFormat::R8") return C78E::Image::ImageFormat(R8);
		if (str == "ImageFormat::RG8") return C78E::Image::ImageFormat::RG8;
		if (str == "ImageFormat::RGB8") return C78E::Image::ImageFormat::RGB8;
		if (str == "ImageFormat::RGBA8") return C78E::Image::ImageFormat::RGBA8;
		if (str == "ImageFormat::RGBA32F") return C78E::Image::ImageFormat::RGBA32F;
		if (str == "ImageFormat::R32") return C78E::Image::ImageFormat::R32;
		if (str == "ImageFormat::D24S8") return C78E::Image::ImageFormat::D24S8;
		return ImageFormat::None;
	}

	Image::Image()
		: m_Width(1), m_Height(1), m_Format(ImageFormat::R8), m_Data(ScopedBuffer(1)) {
		(*m_Data.as<uint8_t>()) = 0xFF;
	}

	Image::Image(uint32_t width, uint32_t height, ImageFormat format, const void* data)
		: m_Width(width), m_Height(height), m_Format(format), m_Data(getByteSize(), data) {
	}

	Image::Image(const Image& copy)
		: m_Width(copy.m_Width), m_Height(copy.m_Height), m_Format(copy.m_Format), m_Data(copy.m_Data) {
		C78E_CORE_ASSERT(m_Data, "Failed to copy Image!");
	}

	Image::~Image() { }


	size_t Image::getByteSize() const {
		if ((uint8_t)m_Format <= 4) return static_cast<size_t>(m_Width * m_Height * (uint8_t)m_Format);
		if (m_Format == ImageFormat::RGBA32F) return static_cast<size_t>(m_Width * m_Height * 4 * 4); //RGBA32F -> 4ch * 4Byte
		C78E_CORE_ASSERT(false, "ImageFormat currently not supported!");
		return 0;
	}

	/*
	* Switched to native Buffer -> if these ops are needed reimpl with that
	* 
	* 
	void Image::expandLeft(Image& image) {
		if (!isValid() || !image.isValid()) C78E_CORE_ASSERT("Image is not valid!");
		if (getHeight() != image.getHeight()) C78E_CORE_ASSERT("Images have different Dimensions!");
		if (getFormat() != image.getFormat()) C78E_CORE_ASSERT("Images differ in Channels!")

		size_t compositeSize = getByteSize() + image.getByteSize();
		uint32_t compositeWidth = getWidth() + image.getWidth();
		void* composite = malloc(compositeSize);

		for (uint32_t row = 0; row < getHeight(); row++) {
			memcpy_s((unsigned char*)composite + (row * compositeWidth * (uint32_t)getFormat()),														image.getWidth() * (uint32_t)image.getFormat(),		(unsigned char*)image.getData() + (image.getWidth() * (uint32_t)image.getFormat() * row),		image.getWidth() * (uint32_t)image.getFormat());
			memcpy_s((unsigned char*)composite + (row * compositeWidth * (uint32_t)getFormat()) + (image.getWidth() * (uint32_t)image.getFormat()),		getWidth() * (uint32_t)getFormat(),					(unsigned char*)getData() + (getWidth() * (uint32_t)getFormat() * row),							getWidth() * (uint32_t)getFormat());
		}

		free(m_Data);
		m_Data = composite;
		m_Width = compositeWidth;
	}

	void Image::expandRight(Image& image) {
		if (!isValid() || !image.isValid()) C78E_CORE_ASSERT("Image is not valid!");
		if (getHeight() != image.getHeight()) C78E_CORE_ASSERT("Images have different Dimensions!");
		if (getFormat() != image.getFormat()) C78E_CORE_ASSERT("Images differ in Channels!")

		size_t compositeSize = getByteSize() + image.getByteSize();
		uint32_t compositeWidth = getWidth() + image.getWidth();
		void* composite = malloc(compositeSize);

		for (uint32_t row = 0; row < getHeight(); row++) {
			memcpy_s((unsigned char*)composite + (row * compositeWidth * (uint32_t)getFormat()),													getWidth() * (uint32_t)getFormat(), (unsigned char*)getData() + (getWidth() * (uint32_t)getFormat() * row), getWidth() * (uint32_t)getFormat());
			memcpy_s((unsigned char*)composite + (row * compositeWidth * (uint32_t)getFormat()) + (getWidth() * (uint32_t)getFormat()),				image.getWidth() * (uint32_t)image.getFormat(), (unsigned char*)image.getData() + (image.getWidth() * (uint32_t)image.getFormat() * row), image.getWidth() * (uint32_t)image.getFormat());
		}

		free(m_Data);
		m_Data = composite;
		m_Width = compositeWidth;
	}

	void Image::expandTop(Image& image) {
		if (!isValid() || !image.isValid()) C78E_CORE_ASSERT("Image is not valid!");
		if (getWidth() != image.getWidth()) C78E_CORE_ASSERT("Images have different Dimensions!");
		if (getFormat() != image.getFormat()) C78E_CORE_ASSERT("Images differ in Channels!")

		size_t compositeSize = getByteSize() + image.getByteSize();
		void* composite = malloc(compositeSize);

		memcpy_s((unsigned char*)composite, image.getByteSize(), image.getData(), image.getByteSize());
		memcpy_s((unsigned char*)composite + image.getByteSize(), getByteSize(), getData(), getByteSize());

		free(m_Data);
		m_Data = composite;
		m_Height = getHeight() + image.getHeight();
	}

	void Image::expandBot(Image& image) {
		if (!isValid() || !image.isValid()) C78E_CORE_ASSERT("Image is not valid!");
		if (getWidth() != image.getWidth()) C78E_CORE_ASSERT("Images have different Dimensions!");
		if (getFormat() != image.getFormat()) C78E_CORE_ASSERT("Images differ in Channels!")

			size_t compositeSize = getByteSize() + image.getByteSize();
		void* composite = malloc(compositeSize);

		memcpy_s((unsigned char*)composite, getByteSize(), getData(), getByteSize());
		memcpy_s((unsigned char*)composite + getByteSize(), image.getByteSize(), image.getData(), image.getByteSize());

		free(m_Data);
		m_Data = composite;
		m_Height = getHeight() + image.getHeight();
	}
	*/
	Image Image::croppedCopy(size_t ox, size_t oy, size_t dx, size_t dy) const {
		C78E_CORE_ASSERT(ox + dx <= m_Width, "Cropped Image must be in bounds!");
		C78E_CORE_ASSERT(oy + dy <= m_Height, "Cropped Image must be in bounds!");

		uint32_t pxSize = ((uint8_t)m_Format < 5) ? (uint8_t)m_Format : 4;
		size_t imagesize = static_cast<size_t>(dx * dy * pxSize);
		void* data = malloc(imagesize);

		for (uint32_t y = 0; y < dy; y++)
			if(void* dst = (unsigned char*)data + (y * dx * pxSize))
				std::memcpy(dst, (unsigned char*)m_Data.as<void>() + (ox * pxSize) + ((oy + y) * m_Width * pxSize), dx * pxSize);

		return Image(static_cast<uint32_t>(dx), static_cast<uint32_t>(dy), m_Format, data);
	}

	Image Image::croppedCopy(float ox, float oy, float dx, float dy) const {
		C78E_CORE_ASSERT(ox + dx <= 1.f, "Cropped Image must be in bounds!");
		C78E_CORE_ASSERT(oy + dy <= 1.f, "Cropped Image must be in bounds!");

		size_t originX = static_cast<size_t>(m_Width * ox);
		size_t originY = static_cast<size_t>(m_Height * oy);
		size_t width = static_cast<size_t>(m_Width * dx);
		size_t height = static_cast<size_t>(m_Height * dy);
		
		return croppedCopy(originX, originY, width, height);
	}
}