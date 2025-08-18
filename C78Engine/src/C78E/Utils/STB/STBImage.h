#pragma once
#include <C78E/Core/Image/Image.h>

namespace C78E::STBImage {

	Ref<Image2D> loadImageSDR(FilePath file, bool flipVert = false, ImageFormat desiredFormat = ImageFormat::None, float nativeHDRToSDRGamma = 2.2f, float nativeHDRToSDRScale = 1.f);
	Ref<Image2D> loadImageHDR(FilePath file, bool flipVert = false, ImageFormat desiredFormat = ImageFormat::None, float nativeSDRToHDRGamma = 2.2f, float nativeSDRToHDRScale = 1.f);

}
