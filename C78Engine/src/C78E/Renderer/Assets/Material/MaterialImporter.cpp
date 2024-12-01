#include "C78EPCH.h"
#include "MaterialImporter.h"

#include <C78E/Project/Project.h>

#include <C78E/Utils/Wavefront/WavefrontLoader.h>

namespace C78E {

	Ref<Material> MaterialLoader::importMaterial(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle) {
		FilePath ext = meta.fileSource.extension();

		//if (ext == ".mtl") return WavefrontLoader::loadMaterial(assetDirectory / meta.fileSource);
		
		return Ref<Material>();
	}

	Ref<Material> MaterialLoader::loadWavefrontMaterial(const FilePath& path, const std::string& name) {
		return Ref<Material>();
	}

}