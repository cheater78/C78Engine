#pragma once

namespace C78E {

	class ImageLoader {
	public:
		struct ImageData {
			Buffer data;
			uint32_t width;
			uint32_t height;
			uint32_t bytesPerPixel;
			uint32_t nativeBytesPerPixel;
			bool isHDR;
		};
	public:
		static Ref<ImageData> loadImage(FilePath file, bool flipVert = false, uint32_t desiredBytesPerPixel = 4);

	private:
		static bool isCompatibleSTBI(FilePath file);
		static Ref<ImageData> loadImageSTBI(FilePath file, bool flipVert = false, uint32_t desiredBytesPerPixel = 4);

		//possibly other ImageLoaderLibs in the future
	};

}
