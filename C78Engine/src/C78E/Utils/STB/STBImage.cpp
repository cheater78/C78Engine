#include "C78EPCH.h"
#include "STBImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace C78E::STBImage {

	/**
	 * @brief stb_image supports 8bit per channel only for SDR formats.
	 * @param channels no. of channels in the image
	 * @return the corresponding ImageFormat for SDR images.
	 */
	static ImageFormat getSDRFormatFromByteSize(uint32_t channels) {
		switch (channels) {
			case 1: return ImageFormat::R8;
			case 2: return ImageFormat::RG8;
			case 3: return ImageFormat::RGB8;
			case 4: return ImageFormat::RGBA8;
			default:
				C78E_CORE_ASSERT(false, "ImageLoader::getImageFormatFromBytesPerPixel: Unsupported Bytes Per Pixel: {}", channels);
				return ImageFormat::RGBA8;
		}
	}

	/**
	 * @brief std_image supports 32bit(float) per channel only for HDR formats.
	 * @param channels no. of channels in the image
	 * @return the corresponding ImageFormat for HDR images.
	 */
	static ImageFormat getHDRFormatFromByteSize(uint32_t channels) {
		switch (channels) {
			case 1: return ImageFormat::R32F;
			case 2: return ImageFormat::RG32F;
			case 3: return ImageFormat::RGB32F;
			case 4: return ImageFormat::RGBA32F;
			default:
				C78E_CORE_ASSERT(false, "ImageLoader::getImageFormatFromBytesPerPixel: Unsupported Bytes Per Pixel: {}", channels);
				return ImageFormat::RGBA32F;
		}
		
	}

	static int s_ThreadSafetyWarning = 0;
	
	Ref<Image2D> loadImageSDR(FilePath file, bool flipVert, ImageFormat desiredFormat, float nativeHDRToSDRGamma, float nativeHDRToSDRScale) {
		C78E_CORE_TRACE("ImageLoader::loadImageSDR: Loading Image File: {}", file.string());
		C78E_CORE_VALIDATE(ImageFormat::isSDRFormat(desiredFormat) || desiredFormat == ImageFormat::None, return nullptr, "ImageLoader::loadImageSDR: Unsupported Image Format: {}", std::to_string(desiredFormat));
		Timer timer{};
		int width, height, channels;
		void* data = nullptr;

		// becomes 0 for None, which is also the default for stbimage -> image will be loaded in native format
		int desiredChannels = static_cast<int>(desiredFormat.getElementCount());

		C78E_CORE_VALIDATE(stbi_info(file.string().c_str(), &width, &height, &channels), 
			return nullptr, "ImageLoader::loadImageSDR: File: {} is incompatible with STBImage, at desired Format: {}",
			file.string(), std::to_string(desiredFormat));

		s_ThreadSafetyWarning++;
		C78E_CORE_SOFT_VALIDATE(s_ThreadSafetyWarning == 1, "ImageLoader::loadImageSDR: STBImage is not thread-safe, please ensure that this function is called from a single thread!");

		stbi_hdr_to_ldr_gamma(nativeHDRToSDRGamma);
		stbi_hdr_to_ldr_scale(nativeHDRToSDRScale);
		stbi_set_flip_vertically_on_load(flipVert ? 1 : 0);

		data = stbi_load(file.string().c_str(), &width, &height, &channels, desiredChannels);

		s_ThreadSafetyWarning--;

		if (!data) {
			C78E_CORE_ERROR("ImageLoader::loadImageSDR: Failed to load Image File: {}, Error: {}", file.string(), stbi_failure_reason());
			return nullptr;
		}

		const ImageFormat format = (desiredFormat == ImageFormat::None) ? getSDRFormatFromByteSize(static_cast<uint32_t>(channels)) : desiredFormat;
		const ImageSize size = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		Ref<Image2D> image = createRef<Image2D>(format, size, data);

		free(data);

		C78E_CORE_TRACE("ImageLoader::loadImageSDR:    ByteSize:      {}B", std::to_string(image->getByteSize()));
		C78E_CORE_TRACE("ImageLoader::loadImageSDR:    ImageSize:     {}", std::to_string(image->getSize()));
		C78E_CORE_TRACE("ImageLoader::loadImageSDR:    Format:		  {}", std::to_string(image->getFormat()));
		C78E_CORE_TRACE("ImageLoader::loadImageSDR: Loading Image Successful, Took: {} ms", std::to_string(timer.elapsedMillis()));

		return image;
	}

	Ref<Image2D> loadImageHDR(FilePath file, bool flipVert, ImageFormat desiredFormat, float nativeSDRToHDRGamma, float nativeSDRToHDRScale) {
		C78E_CORE_TRACE("ImageLoader::loadImageHDR: Loading Image File: {}", file.string());
		C78E_CORE_VALIDATE(ImageFormat::isHDRFormat(desiredFormat) || desiredFormat == ImageFormat::None, return nullptr, "ImageLoader::loadImageHDR: Unsupported Image Format: {}", std::to_string(desiredFormat));
		Timer timer{};
		int width, height, channels;
		void* data = nullptr;

		// becomes 0 for None, which is also the default for stbimage -> image will be loaded in native format
		int desiredChannels = static_cast<int>(desiredFormat.getElementCount());

		C78E_CORE_VALIDATE(stbi_info(file.string().c_str(), &width, &height, &channels),
			return nullptr, "ImageLoader::loadImageHDR: File: {} is incompatible with STBImage, at desired Format: {}",
			file.string(), std::to_string(desiredFormat));

		s_ThreadSafetyWarning++;
		C78E_CORE_SOFT_VALIDATE(s_ThreadSafetyWarning == 1, "ImageLoader::loadImageHDR: STBImage is not thread-safe, please ensure that this function is called from a single thread!");

		stbi_ldr_to_hdr_gamma(nativeSDRToHDRGamma);
		stbi_ldr_to_hdr_scale(nativeSDRToHDRScale);
		stbi_set_flip_vertically_on_load(flipVert ? 1 : 0);

		data = stbi_load(file.string().c_str(), &width, &height, &channels, desiredChannels);

		s_ThreadSafetyWarning--;

		if (!data) {
			C78E_CORE_ERROR("ImageLoader::loadImageHDR: Failed to load Image File: {}, Error: {}", file.string(), stbi_failure_reason());
			return nullptr;
		}

		const ImageFormat format = (desiredFormat == ImageFormat::None) ? getSDRFormatFromByteSize(static_cast<uint32_t>(channels)) : desiredFormat;
		const ImageSize size = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		Ref<Image2D> image = createRef<Image2D>(format, size, data);

		free(data);

		C78E_CORE_TRACE("ImageLoader::loadImageHDR:    ByteSize:      {}B", std::to_string(image->getByteSize()));
		C78E_CORE_TRACE("ImageLoader::loadImageHDR:    ImageSize:     {}", std::to_string(image->getSize()));
		C78E_CORE_TRACE("ImageLoader::loadImageHDR:    Format:		  {}", std::to_string(image->getFormat()));
		C78E_CORE_TRACE("ImageLoader::loadImageHDR: Loading Image Successful, Took: {} ms", std::to_string(timer.elapsedMillis()));

		return image;
	}


}


