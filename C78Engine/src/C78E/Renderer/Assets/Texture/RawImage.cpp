#include <C78ePCH.h>
#include "RawImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



namespace C78E {

	RawImage::RawImage(const char* file, bool flipvert)
		: m_Name(file)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(flipvert);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(file, &width, &height, &channels, 0);
		}

		m_Format = (ImageFormat)static_cast<uint32_t>(channels);
		m_Width = static_cast<uint32_t>(width);
		m_Height = static_cast<uint32_t>(height);

		size_t imagesize = getByteSize();
		m_Data = malloc(imagesize);

		memcpy_s(m_Data, imagesize, data, imagesize);

		stbi_image_free(data);
	}

	RawImage::RawImage(const RawImage& copy)
		:m_Name(copy.m_Name), m_Width(copy.m_Width), m_Height(copy.m_Height), m_Format(copy.m_Format)
	{
		m_Data = malloc(static_cast<size_t>(m_Width * m_Height * (uint32_t)m_Format));
		memcpy_s(m_Data, static_cast<size_t>(m_Width * m_Height * (uint32_t)m_Format), copy.m_Data, static_cast<size_t>(m_Width * m_Height * (uint32_t)m_Format));
		C78_CORE_ASSERT(m_Data, "Failed to copy RawImage!");
	}

	RawImage::~RawImage() {
		free(m_Data);
	}


	size_t RawImage::getByteSize() {
		if ((uint32_t)m_Format <= 4) return static_cast<size_t>(m_Width * m_Height * (uint32_t)m_Format);
		C78_CORE_ASSERT("ImageFormat currently not supported!");
	}

	void RawImage::expandLeft(RawImage& image) {
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

	void RawImage::expandRight(RawImage& image) {
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

	void RawImage::expandTop(RawImage& image) {
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

	void RawImage::expandBot(RawImage& image) {
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



}