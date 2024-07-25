#include "C78ePCH.h"
#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace C78E {

	Ref<Image> ImageLoader::load(const FilePath& imageFile, bool flipVert, uint32_t channelByteAlign) {
		C78_CORE_TRACE("Loading Image: {}", imageFile.string());
		int width, height, channels;
		uint8_t* data = nullptr;

		C78_CORE_TRACE("  flipVert: {}", (flipVert) ? "true" : "false");
		stbi_set_flip_vertically_on_load(flipVert);

		data = stbi_load(imageFile.string().c_str(), &width, &height, &channels, channelByteAlign);
		if (data) {
			C78_CORE_TRACE("  loaded: true");
			C78_CORE_TRACE("  width: {}", width);
			C78_CORE_TRACE("  height: {}", height);
			C78_CORE_TRACE("  channels: {}", channels);

			Image::ImageFormat imageFormat = Image::ImageFormat::None;
			switch (channels) {
			case 1: imageFormat = Image::ImageFormat::R8; break;
			case 2: imageFormat = Image::ImageFormat::RG8; break;
			case 3: imageFormat = Image::ImageFormat::RGB8; break;
			case 4: imageFormat = Image::ImageFormat::RGBA8; break;
			}

			Ref<Image> image = createRef<Image>(width, height, imageFormat, data);
			delete[] data;
			return image;

		} else {
			C78_CORE_ERROR("  loaded: failed");
			C78_CORE_ERROR("TextureLoader::loadTexture2D - Could not load texture from filepath: {}", imageFile.string());
			return nullptr;
		}
	}

}
