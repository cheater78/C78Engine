#include "C78EPCH.h"
#include "ImageLoader.h"

#include <C78E/Core/Buffer.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace C78E {

	/*
	* Loads an Image from Disk
	*/
	Ref<Image> ImageLoader::loadImage(FilePath file, bool flipVert, uint32_t desiredChannels) {
		if (isCompatibleSTBI(file))
			return loadImageSTBI(file, flipVert, desiredChannels);
		else {
			C78E_CORE_ERROR("ImageLoader::loadImage: Unsupported File: {}", file.string());
			C78E_CORE_ASSERT(false);
			return nullptr;
		}
	}

	/*
	* Checks compatibility with STBI
	*/
	bool ImageLoader::isCompatibleSTBI(FilePath file) {
		int w, h, bpp;
		return stbi_info(file.string().c_str(), &w, &h, &bpp);
	}

	/*
	* Loads an Image from Disk using STBI
	*/
	Ref<Image> ImageLoader::loadImageSTBI(FilePath file, bool flipVert, uint32_t desiredBytesPerPixel) {
		C78E_CORE_TRACE("ImageLoader::loadImageSTBI: Loading Image File: {}", file.string());
		Timer timer{};
		int width, height, bytesPerPixel;

		if(flipVert) stbi_set_flip_vertically_on_load(1);
		else stbi_set_flip_vertically_on_load(0);

		void* data = stbi_load(file.string().c_str(), &width, &height, &bytesPerPixel, static_cast<int>(desiredBytesPerPixel));

		if (data) {
			C78E_CORE_VALIDATE(bytesPerPixel < 6, return nullptr, "ImageLoader::loadImageSTBI: Failed to load Image File: {}, Format undetectable!", file.string());
			Image::ImageFormat format = (bytesPerPixel < 5) ? (uint8_t)bytesPerPixel : Image::ImageFormat::RGBA32F;
			Ref<Image> image = createRef<Image>(width, height, format, data);

			C78E_CORE_TRACE("ImageLoader::loadImageSTBI:    Size:          {}B", std::to_string(image->getByteSize()));
			C78E_CORE_TRACE("ImageLoader::loadImageSTBI:    Width:         {}", std::to_string(image->getWidth()));
			C78E_CORE_TRACE("ImageLoader::loadImageSTBI:    Height:        {}", std::to_string(image->getHeight()));
			C78E_CORE_TRACE("ImageLoader::loadImageSTBI:    Format:		   {}", std::to_string(image->getFormat()));
			C78E_CORE_TRACE("ImageLoader::loadImageSTBI: Loading Image Successful, Took: {} ms", std::to_string(timer.elapsedMillis()));

			return image;
		}

		C78E_CORE_ERROR("ImageLoader::loadImageSTBI: Failed to load Image File: {}", file.string());
		return nullptr;
	}

}

