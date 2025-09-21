#pragma once

namespace C78E {

	using ImagePosition = uvec2;
	using ImageSize = uvec2;

	class ImageFormat {
	public:
		enum ImagePixelLayout {
			None = 0, // no pixel layout defined

			R8 = 1, // single channel, 8 bits -> 1B
			R16F = 2, // single channel, 16 bits (floating point) -> 2B
			R32 = 3, // single channel, 32 bits (integer) -> 4B
			R32F = 4, // single channel, 32 bits (floating point) -> 4B
			D24S8 = 5, // depth 24 bits, stencil 8 bits -> 4B

			RG8 = 6, // 2 channels, 8 bits per channel -> 2B
			RG16F = 7, // 2 channels, 16 bits per channel (floating point) -> 4B
			RG32F = 8, // 2 channels, 32 bits per channel (floating point) -> 8B

			RGB8 = 9, // 3 channels, 8 bits per channel -> 3B
			RGB16F = 10, // 3 channels, 16 bits per channel (floating point) -> 6B
			RGB32F = 11, // 3 channels, 32 bits per channel (floating point) -> 12B

			RGBA8 = 12, // 4 channels, 8 bits per channel -> 4B
			RGBA16F = 13, // 4 channels, 16 bits per channel (floating point) -> 8B
			RGBA32F = 14, // 4 channels, 32 bits per channel (floating point) -> 16B

			BGRA8S, // sRGB! 4 channels, 8 bits per channel -> 4B
		};
	public:
		static std::string imageFormatToString(ImageFormat imageFormat);
		static ImageFormat imageFormatFromString(const std::string& imageFormatString);
		static size_t imageFormatByteSize(ImagePixelLayout imagePixelLayout);
		static size_t imageFormatElementCount(ImagePixelLayout imagePixelLayout);
		static size_t imageFormatBitsPerElement(ImagePixelLayout imagePixelLayout);

		static bool isDepthFormat(ImageFormat imageFormat);
		static bool isColorFormat(ImageFormat imageFormat);
		static bool isSDRFormat(ImageFormat imageFormat);
		static bool isHDRFormat(ImageFormat imageFormat);
	public:
		ImageFormat();
		ImageFormat(ImagePixelLayout imagePixelLayout);
		~ImageFormat();

		size_t getElementCount() const;
		size_t getBitsPerElement() const;
		size_t getByteSize() const;

		ImagePixelLayout format() const;

		bool operator==(const ImageFormat& other) const;
		bool operator!=(const ImageFormat& other) const;

	private:
		ImagePixelLayout m_ImagePixelLayout = None;
	};


	enum class ColorSpace {
		sRGB,
		sRGBLinear,

	};

}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::ImageFormat& imageFormat) {
		return C78E::ImageFormat::imageFormatToString(imageFormat);
	}

}
