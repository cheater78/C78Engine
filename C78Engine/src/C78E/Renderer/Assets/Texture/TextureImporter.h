#pragma once

#include <C78E/Renderer/API/Texture.h>

namespace C78E {

	class TextureImporter {
	public:
		static Ref<Asset::Group> importTexture2D(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle);
		static Ref<Asset::Group> importCubeMap(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle);
	};

}