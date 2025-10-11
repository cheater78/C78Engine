#pragma once
#include "C78EPCH.h"
#include <C78E/Core/Buffer.h>

namespace C78E {

	/*
	* CPU Memory Image
	*/
	class Image {
	public:
		enum class Channel : uint8_t {
			None = 0,
			R = 1,
			G = 2,
			B = 3,
			A = 4,
			L = 5,
			M = 6,
			Z = 7,
		};
		class ImageFormat {
		public:
			enum : uint8_t {
				None = 0,
				R8 = 1,
				RG8 = 2,
				RGB8 = 3,
				RGBA8 = 4,
				RGBA32F = 5,
				R32 = 6,
				D24S8 = 7
			};
			ImageFormat() : m_Format{ 0 } {}
			ImageFormat(uint8_t format) : m_Format(format) {}
			ImageFormat(const ImageFormat& other) = default;
			~ImageFormat() = default;

			bool operator==(const ImageFormat& other) const { return this->m_Format == other.m_Format; }
			bool operator==(uint8_t other) const { return this->m_Format == other; }
			bool operator!=(const ImageFormat& other) const { return this->m_Format != other.m_Format; }
			bool operator!=(uint8_t other) const { return this->m_Format != other; }
			operator uint8_t() const { return m_Format; }
			operator bool() const { return m_Format; }

			static const size_t imageFormatSize(const ImageFormat& imageFormat);
			static std::string imageFormatToString(const ImageFormat& imageFormat);
			static ImageFormat imageFormatFromString(const std::string& str);
		private:
			uint8_t m_Format;
		};

	public:
		Image(); // single pixel, single channel, white Image
		Image(uint32_t width, uint32_t height, ImageFormat format, const void* data);
		Image(const Image& copy);
		~Image();

		bool isValid() const { return m_Data; }
		void* getData() const { return m_Data.as<void>(); }
		ScopedBuffer& getDataBuffer() { return m_Data; }
		const ScopedBuffer& getDataBuffer() const { return m_Data; }

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
		Image croppedCopy(float ox, float oy, float dx, float dy) const;
		Image croppedCopy(size_t ox, size_t oy, size_t dx, size_t dy) const;
		
	private:
		uint32_t m_Width = 1;
		uint32_t m_Height = 1;
		ImageFormat m_Format = ImageFormat(ImageFormat::R8);

		ScopedBuffer m_Data;
	};
}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::Image::ImageFormat& imageFormat) {
		return C78E::Image::ImageFormat::imageFormatToString(imageFormat);
	}

}