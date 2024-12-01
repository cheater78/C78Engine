#pragma once
#include "C78EPCH.h"
#include <C78E/Core/Buffer.h>

namespace C78E {

	/*
	* CPU Memory Image
	*/
	class Image {
	public:
		enum class ImageFormat : uint32_t {
			None = 0,
			R8 = 1,
			RG8 = 2,
			RGB8 = 3,
			RGBA8 = 4,
			RGBA32F = 5,
			R32 = 6,
			D24S8 = 7
		};
		static const size_t imageFormatSize(ImageFormat format) {
			if ((uint32_t)format <= 4) return (uint32_t)format;
			else if (format == ImageFormat::RGBA32F) return 16;
			else if (format == ImageFormat::R32) return 4;
			else if (format == ImageFormat::D24S8) return 4;
			else C78E_CORE_ERROR("Image::imageFormatSize: illegal ImageFormat!"); return 0;
		}
		static std::string imageFormatToString(ImageFormat format);
		static ImageFormat imageFormatFromString(const std::string& str);

	public:
		Image() = delete; //TODO: Check
		Image(uint32_t width, uint32_t height, ImageFormat format, const void* data);
		Image(const Image& copy);
		~Image();

		bool isValid() const { return m_Data; }
		void* getData() const { return m_Data.data; }

		uint32_t getWidth() const { return m_Width; }
		uint32_t getHeight() const { return m_Height; }
		ImageFormat getFormat() const { return m_Format; }

		size_t getByteSize() const;

		/*
		void expandLeft(Image& image);
		void expandRight(Image& image);
		void expandTop(Image& image);
		void expandBot(Image& image);
		*/
		Ref<Image> croppedCopy(float ox, float oy, float dx, float dy);
		Ref<Image> croppedCopy(uint32_t ox, uint32_t oy, uint32_t dx, uint32_t dy);

	private:
		uint32_t m_Width = 1;
		uint32_t m_Height = 1;
		ImageFormat m_Format = ImageFormat::R8;

		Buffer m_Data;

	};
}