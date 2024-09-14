#pragma once

#include <C78E/Renderer/API/Texture.h>

namespace C78E {

	class TextureImporter {
	public:
		/*
		* Imports the Texture specified in meta.fileSource
		* returns a reference to the Texture
		*/
		static Ref<Texture2D> importTexture2D(AssetHandle handle, const Asset::AssetMeta& meta);

		//Temp: TODO: remove
		static Ref<Texture2D> loadImageFile(FilePath file);
	};



}