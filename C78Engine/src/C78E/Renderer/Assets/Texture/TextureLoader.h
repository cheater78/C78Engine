#pragma once

namespace C78E {

	class TextureLoader {
	public:
		/*
		* Imports the Texture specified in meta.fileSource
		* returns a reference to the Texture
		*/
		static Ref<Texture2D> importTexture2D(AssetHandle handle, const Asset::AssetMeta& meta);

		
		//static AssetHandle loadTexture2D();

	public:
		static Ref<Texture2D> loadImageFile(const FilePath& path);
	};



}