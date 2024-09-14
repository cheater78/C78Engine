#include "C78ePCH.h"
#include "CubeMapImporter.h"

#include <C78E/Renderer/Assets/Texture/Image.h>
#include <C78E/Utils/Image/ImageLoader.h>

namespace C78E {
	
	Ref<CubeMap> CubeMapImporter::importCubeMap(AssetHandle handle, const Asset::AssetMeta& meta) {
		Ref<ImageLoader::ImageData> image = ImageLoader::loadImage(meta.fileSource, false, 4);

		C78E::Texture2D::TextureSpecification spec;
		spec.width = image->width;
		spec.height = image->height;
		switch (image->bytesPerPixel) {
		case 1: spec.format = Image::ImageFormat::R8; break;
		case 2: spec.format = Image::ImageFormat::RG8; break;
		case 3: spec.format = Image::ImageFormat::RGB8; break;
		case 4: spec.format = Image::ImageFormat::RGBA8; break;
		case 16: spec.format = Image::ImageFormat::RGBA32F; break;
		}
		if (spec.format == Image::ImageFormat::RGBA32F)
			C78_CORE_ASSERT(image->isHDR, "TextureImporter::importTexture2D: Image has 16 Bytes per Pixel but was not flagged HDR!");

		return CubeMap::create(createRef<Image>(spec.width, spec.height, spec.format, image->data.as<void>()));
	}

}
