#include "C78ePCH.h"
#include "TextureLoader.h"

#include "C78E/Core/Buffer.h"
#include <C78E/Project/Project.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace C78E {

	Ref<Texture2D> TextureLoader::importTexture2D(AssetHandle handle, const Asset::AssetMeta& meta) {
		return loadTexture2D(Project::getActiveAssetDirectory() / meta.fileSource);
	}

	Ref<Texture2D> TextureLoader::loadTexture2D(const FilePath& path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		Buffer data;

		{
			std::string pathStr = path.string();
			data.data = stbi_load(pathStr.c_str(), &width, &height, &channels, 4);
			channels = 4;
		}

		if (data.data == nullptr) {
			C78_CORE_ERROR("TextureLoader::loadTexture2D - Could not load texture from filepath: {}", path.string());
			return nullptr;
		}

		// TODO: rework Format handling
		data.size = width * height * channels;

		C78E::Texture2D::TextureSpecification spec;
		spec.width = width;
		spec.height = height;
		switch (channels) {
		case 3: spec.format = Image::ImageFormat::RGB8; break;
		case 4: spec.format = Image::ImageFormat::RGBA8; break;
		}

		Ref<Texture2D> texture = Texture2D::create(spec, data);
		data.release();
		return texture;
	}

}