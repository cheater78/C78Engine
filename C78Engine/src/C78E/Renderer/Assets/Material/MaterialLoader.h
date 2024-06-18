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
		static Ref<Material> importMaterial(AssetHandle handle, const Asset::AssetMeta& meta);

		// Load from filepath
		static Ref<Material> loadMaterial(const FilePath& path);
	private:
		static Ref<Material> loadWavefrontMaterial(const FilePath& path, const std::string& name);
	};

}