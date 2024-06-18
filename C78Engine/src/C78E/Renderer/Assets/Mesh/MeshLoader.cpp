#include "C78ePCH.h"
#include "MeshLoader.h"

#include <C78E/Project/Project.h>

namespace C78E {

	Ref<Mesh> MeshLoader::importMesh(AssetHandle handle, const Asset::AssetMeta& meta) {
		//return loadMesh(Project::getActiveAssetDirectory() / meta.fileSource);
		return Ref<Mesh>();
	}

	Ref<Mesh> MeshLoader::loadMesh(const FilePath& path) {

		// TODO
		C78_CORE_ERROR("Impl! MeshLoader");

		return Ref<Mesh>();
	}

}