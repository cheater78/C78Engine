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
		static Ref<Asset::Group> importMaterial(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle);
	};

}