#include "C78ePCH.h"
#include "TextureLoader.h"

#include <C78E/Utils/Image/ImageLoader.h>
#include <C78E/Renderer/API/Texture.h>

#include <C78E/Renderer/API/RendererAPI.h>

namespace C78E {

	Ref<Texture2D> TextureLoader::importTexture2D(AssetHandle handle, const Asset::AssetMeta& meta) {
		FilePath textureFile = meta.fileSource;
		FilePath textureExtension = textureFile.extension();

		Ref<Image> image = nullptr;
		bool flipVert = false;
		uint32_t channelByteAlign = 0;

		switch (RendererAPI::getAPI()) {
		case RendererAPI::API::OpenGL:
			flipVert = true;
			channelByteAlign = 4;
			break;
		case RendererAPI::API::Vulkan:
			flipVert = false;
			break;
		default: break;
		}

		//TODO: hdr is stbi supported too, check
		if (textureExtension == ".png" ||
			textureExtension == ".jpg" ||
			textureExtension == ".jpeg" ||
			textureExtension == ".psd" ||
			textureExtension == ".bmp" ||
			textureExtension == ".gif" ||
			textureExtension == ".pnm" ) {
			image = ImageLoader::load(textureFile, flipVert, channelByteAlign);
		} else {
			C78_CORE_ERROR("TextureLoader::importTexture2D: File: '{}' not supported!", textureFile.string().c_str());
			C78_CORE_ERROR("  extension: {}", textureExtension.string().c_str());
			C78_CORE_ASSERT(false);
			return nullptr;
		}

		return Texture2D::create(*image);
	}

}