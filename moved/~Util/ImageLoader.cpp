#include "C78ePCH.h"
#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace C78e {
	namespace Util {
		RawImage loadImage(const std::string file) {
			RawImage img;

			img.data = stbi_load(file.c_str(), &img.width, &img.height, &img.channels, STBI_rgb_alpha);

			if (!img.data) {
				throw std::runtime_error("failed to load texture image!");
			}

			return img;
		}
	}
}