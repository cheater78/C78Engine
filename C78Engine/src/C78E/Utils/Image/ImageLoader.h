#pragma once

#include <C78E/Renderer/Assets/Texture/Image.h>

namespace C78E {

	/*
	* ImageLoader using stbimage
	*/
	class ImageLoader {
	public:
		static Ref<Image> load(const FilePath& imageFile, bool flipVert = false, uint32_t channelByteAlign = 0);

	};

}
