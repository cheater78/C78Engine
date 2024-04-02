#pragma once
#include "C78E/Assets/Asset/Asset.h"

#include "Texture.h"

namespace C78E {

	class TextureLoader {
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Texture2D> importTexture2D(AssetHandle handle, const Asset::AssetMeta& meta);

		// Reads file directly from filesystem
		// (i.e. path has to be relative / absolute to working directory)
		static Ref<Texture2D> loadTexture2D(const FilePath& path);
	};



}