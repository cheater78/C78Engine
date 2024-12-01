#include "C78EPCH.h"
#include "TextureImporter.h"

#include <C78E/Core/Buffer.h>
#include <C78E/Renderer/API/Texture.h>

#include <C78E/Utils/Image/ImageLoader.h>

namespace C78E {

	Ref<Texture2D> TextureImporter::importTexture2D(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle) {

		Ref<ImageLoader::ImageData> image = ImageLoader::loadImage(meta.fileSource, true, 4);

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
			C78E_CORE_ASSERT(image->isHDR, "TextureImporter::importTexture2D: Image has 16 Bytes per Pixel but was not flagged HDR!");

		return Texture2D::create(spec, image->data);
	}

	Ref<CubeMap> TextureImporter::importCubeMap(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle) {
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
			C78E_CORE_ASSERT(image->isHDR, "TextureImporter::importTexture2D: Image has 16 Bytes per Pixel but was not flagged HDR!");

		return CubeMap::create(createRef<Image>(spec.width, spec.height, spec.format, image->data.as<void>()));
	}
	
}