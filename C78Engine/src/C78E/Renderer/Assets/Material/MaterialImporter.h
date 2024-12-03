#pragma once
#include "Material.h"

namespace C78E {

	/*
	* MaterialLoader
	* Wavefront -> 
	*/
	class MaterialLoader {
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Material> importMaterial(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle);
		
	private:
		static Ref<Material> loadWavefrontMaterial(const FilePath& path, const std::string& name);
	};

}