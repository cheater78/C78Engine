#include "C78ePCH.h"
#include "MaterialLoader.h"

#include <C78E/Project/Project.h>

#include <C78E/Utils/Wavefront/WavefrontLoader.h>

namespace C78E {

	Ref<Material> MaterialLoader::importMaterial(AssetHandle handle, const Asset::AssetMeta& meta) {
		FilePath ext = meta.fileSource.extension();

		//if (ext == ".mtl") return WavefrontLoader::loadMaterial(Project::getActiveAssetDirectory() / meta.fileSource, meta.name);

		//return loadMaterial(Project::getActiveAssetDirectory() / meta.fileSource);
		return Ref<Material>();
	}

	Ref<Material> MaterialLoader::loadMaterial(const FilePath& path) {

		// TODO
		C78_CORE_ERROR("Impl! MaterialLoader");

		return Ref<Material>();
	}

	Ref<Material> MaterialLoader::loadWavefrontMaterial(const FilePath& path, const std::string& name) {
		return Ref<Material>();
	}

}