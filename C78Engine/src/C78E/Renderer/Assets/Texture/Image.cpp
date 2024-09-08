#include <C78ePCH.h>
#include "Image.h"

namespace C78E {

	Image::Image(uint32_t width, uint32_t height, ImageFormat format, const  void* data)
		: m_Width(width), m_Height(height), m_Format(format) {
		size_t imagesize = getByteSize();
		m_Data = Buffer(imagesize);
		memcpy_s(m_Data.data, imagesize, data, imagesize);
	}

	Image::Image(const Image& copy)
		:m_Width(copy.m_Width), m_Height(copy.m_Height), m_Format(copy.m_Format) {
		m_Data.copy(copy.m_Data);
		C78_CORE_ASSERT(m_Data, "Failed to copy Image!");
	}

	Image::~Image() { }


	size_t Image::getByteSize() const {
		if ((uint32_t)m_Format <= 4) return static_cast<size_t>(m_Width * m_Height * (uint32_t)m_Format);
		if (m_Format == ImageFormat::RGBA32F) return static_cast<size_t>(m_Width * m_Height * 4 * 4); //RGBA32F -> 4ch á 4Byte
		C78_CORE_ASSERT("ImageFormat currently not supported!");
		return 0;
	}

	/*
	* Switched to native Buffer -> if these ops are needed reimpl with that
	* 
	* 
	void Image::expandLeft(Image& image) {
		if (!isValid() || !image.isValid()) C78_CORE_ASSERT("Image is not valid!");
		if (getHeight() != image.getHeight()) C78_CORE_ASSERT("Images have different Dimensions!");
		if (getFormat() != image.getFormat()) C78_CORE_ASSERT("Images differ in Channels!")

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
		if (!isValid() || !image.isValid()) C78_CORE_ASSERT("Image is not valid!");
		if (getHeight() != image.getHeight()) C78_CORE_ASSERT("Images have different Dimensions!");
		if (getFormat() != image.getFormat()) C78_CORE_ASSERT("Images differ in Channels!")

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
		if (!isValid() || !image.isValid()) C78_CORE_ASSERT("Image is not valid!");
		if (getWidth() != image.getWidth()) C78_CORE_ASSERT("Images have different Dimensions!");
		if (getFormat() != image.getFormat()) C78_CORE_ASSERT("Images differ in Channels!")

		size_t compositeSize = getByteSize() + image.getByteSize();
		void* composite = malloc(compositeSize);

		memcpy_s((unsigned char*)composite, image.getByteSize(), image.getData(), image.getByteSize());
		memcpy_s((unsigned char*)composite + image.getByteSize(), getByteSize(), getData(), getByteSize());

		free(m_Data);
		m_Data = composite;
		m_Height = getHeight() + image.getHeight();
	}

	void Image::expandBot(Image& image) {
		if (!isValid() || !image.isValid()) C78_CORE_ASSERT("Image is not valid!");
		if (getWidth() != image.getWidth()) C78_CORE_ASSERT("Images have different Dimensions!");
		if (getFormat() != image.getFormat()) C78_CORE_ASSERT("Images differ in Channels!")

			size_t compositeSize = getByteSize() + image.getByteSize();
		void* composite = malloc(compositeSize);

		memcpy_s((unsigned char*)composite, getByteSize(), getData(), getByteSize());
		memcpy_s((unsigned char*)composite + getByteSize(), image.getByteSize(), image.getData(), image.getByteSize());

		free(m_Data);
		m_Data = composite;
		m_Height = getHeight() + image.getHeight();
	}
	*/
	Ref<Image> Image::croppedCopy(uint32_t ox, uint32_t oy, uint32_t dx, uint32_t dy)
	{
		C78_CORE_ASSERT(ox + dx <= m_Width, "Cropped Image must be in bounds!");
		C78_CORE_ASSERT(oy + dy <= m_Height, "Cropped Image must be in bounds!");

		uint32_t pxSize = ((uint32_t)m_Format < 5) ? (uint32_t)m_Format : 4;
		size_t imagesize = static_cast<size_t>(dx * dy * pxSize);
		void* data = malloc(imagesize);

		for (uint32_t y = 0; y < dy; y++) {
			memcpy_s((unsigned char*)data + (y * dx * pxSize), dx * pxSize, (unsigned char*)m_Data.data + (ox * pxSize) + ((oy + y) * m_Width * pxSize), dx * pxSize);
		}

		return createRef<Image>(dx, dy, m_Format, data);
	}

	Ref<Image> Image::croppedCopy(float ox, float oy, float dx, float dy)
	{
		C78_CORE_ASSERT(ox + dx <= 1.f, "Cropped Image must be in bounds!");
		C78_CORE_ASSERT(oy + dy <= 1.f, "Cropped Image must be in bounds!");

		uint32_t originX = static_cast<uint32_t>(m_Width * ox);
		uint32_t originY = static_cast<uint32_t>(m_Height * oy);
		uint32_t width = static_cast<uint32_t>(m_Width * dx);
		uint32_t height = static_cast<uint32_t>(m_Height * dy);
		
		return croppedCopy(originX, originY, width, height);
	}
	
}