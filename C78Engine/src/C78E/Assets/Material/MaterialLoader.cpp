#include "C78ePCH.h"
#include "MaterialLoader.h"

#include <C78E/Project/Project.h>

namespace C78E {

	Ref<Material> MaterialLoader::importMaterial(AssetHandle handle, const Asset::AssetMeta& meta) {
		return loadMaterial(Project::getActiveAssetDirectory() / meta.fileSource);
	}

	Ref<Material> MaterialLoader::loadMaterial(const FilePath& path) {

		// TODO
		C78_CORE_ERROR("Impl! MaterialLoader");

		return Ref<Material>();
	}

}