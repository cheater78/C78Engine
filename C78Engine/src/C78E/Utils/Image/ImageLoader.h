#pragma once

#include <C78E/Asset/Texture/Image.h>

namespace C78E {

	class ImageLoader {
	public:
		static Ref<Image> loadImage(FilePath file, bool flipVert = false, uint32_t desiredBytesPerPixel = 4);

	private:
		static bool isCompatibleSTBI(FilePath file);
		static Ref<Image> loadImageSTBI(FilePath file, bool flipVert = false, uint32_t desiredBytesPerPixel = 4);

		//possibly other ImageLoaderLibs in the future
	};

}
