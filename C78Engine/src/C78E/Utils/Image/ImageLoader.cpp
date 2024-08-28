#include "C78ePCH.h"
#include "ImageLoader.h"

#include <C78E/Core/Buffer.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace C78E {

	/*
	* Loads an Image from Disk into a ImageLoader::ImageData Transfer Object
	*/
	Ref<ImageLoader::ImageData> ImageLoader::loadImage(FilePath file, bool flipVert, uint32_t desiredChannels) {
		if (isCompatibleSTBI(file))
			return loadImageSTBI(file, flipVert, desiredChannels);
		else {
			C78_CORE_ERROR("ImageLoader::loadImage: Unsupported File: {}", file.string());
			C78_CORE_ASSERT(false);
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
	Ref<ImageLoader::ImageData> ImageLoader::loadImageSTBI(FilePath file, bool flipVert, uint32_t desiredBytesPerPixel) {
		C78_CORE_TRACE("ImageLoader::loadImageSTBI: Loading Image File: {}", file.string());
		Timer timer{};
		Ref<ImageLoader::ImageData> image = createRef<ImageLoader::ImageData>();
		int width, height, bytesPerPixel;

		if(flipVert) stbi_set_flip_vertically_on_load(1);
		else stbi_set_flip_vertically_on_load(0);

		image->data.data = stbi_load(file.string().c_str(), &width, &height, &bytesPerPixel, static_cast<int>(desiredBytesPerPixel));

		if (image->data.data) {
			image->data.size = width * height * bytesPerPixel;
			image->width = width;
			image->height = height;
			image->bytesPerPixel = desiredBytesPerPixel;
			image->nativeBytesPerPixel = bytesPerPixel;

			image->isHDR = stbi_is_hdr(file.string().c_str());


			C78_CORE_TRACE("ImageLoader::loadImageSTBI:    Size:          {}", std::to_string(image->data.size));
			C78_CORE_TRACE("ImageLoader::loadImageSTBI:    Width:         {}", std::to_string(image->width));
			C78_CORE_TRACE("ImageLoader::loadImageSTBI:    Height:        {}", std::to_string(image->height));
			C78_CORE_TRACE("ImageLoader::loadImageSTBI:    BytesPerPixel: {}", std::to_string(image->bytesPerPixel));
			C78_CORE_TRACE("ImageLoader::loadImageSTBI:    NativeBytesPP: {}", std::to_string(image->nativeBytesPerPixel));
			C78_CORE_TRACE("ImageLoader::loadImageSTBI:    HDR:           {}", std::to_string(image->isHDR));
			C78_CORE_TRACE("ImageLoader::loadImageSTBI: Loading Image Successful, Took: {} ms", std::to_string(timer.elapsedMillis()));

			return image;
		}

		C78_CORE_ERROR("ImageLoader::loadImageSTBI: Failed to load Image File: {}", file.string());
		return nullptr;
	}

}

