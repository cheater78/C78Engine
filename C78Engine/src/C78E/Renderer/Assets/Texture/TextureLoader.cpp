#include "C78ePCH.h"
#include "TextureLoader.h"

#include <C78E/Core/Buffer.h>
#include <C78E/Project/Project.h>

#include <C78E/Renderer/API/RendererAPI.h>
#include <C78E/Renderer/API/Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace C78E {

	Ref<Texture2D> TextureLoader::importTexture2D(AssetHandle handle, const Asset::AssetMeta& meta) {
		return loadImageFile(meta.fileSource);
	}

	Ref<Texture2D> TextureLoader::loadImageFile(const FilePath& path) {
		int width, height, channels;
		Buffer data;

		if (C78E::RendererAPI::getAPI() == C78E::RendererAPI::API::OpenGL) {
			stbi_set_flip_vertically_on_load(0);
		} else if (C78E::RendererAPI::getAPI() == C78E::RendererAPI::API::Vulkan) {
			C78_CORE_WARN("TextureLoader::loadImageFile: flipping Image for Vulkan, check me!");
			stbi_set_flip_vertically_on_load(1);
		}
		

		// TODO: rework Format handling -> 3/4 channels become 4ch -> 4Byte aligned
		{
			std::string pathStr = path.string();
			data.data = stbi_load(pathStr.c_str(), &width, &height, &channels, 4);
			C78_CORE_WARN("TextureLoader::loadImageFile: not setting 4ch, but req is 4, check me!");
			//channels = 4;
		}

		if (data.data == nullptr) {
			C78_CORE_ERROR("TextureLoader::loadTexture2D - Could not load texture from filepath: {}", path.string());
			return nullptr;
		}

		
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