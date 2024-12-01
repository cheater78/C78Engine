#pragma once

#include <C78E/Renderer/API/Texture.h>

namespace C78E {

	class TextureImporter {
	public:
		static Ref<Texture2D> importTexture2D(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle);
		static Ref<CubeMap> importCubeMap(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle);
	};

}