#include "C78ePCH.h"
#include "ModelLoader.h"

#include <C78E/Project/Project.h>


namespace C78E {

	Ref<Model> ModelLoader::importModel(AssetHandle handle, const Asset::AssetMeta& meta) {
		return loadModel(Project::getActiveAssetDirectory() / meta.fileSource);
	}

	Ref<Model> ModelLoader::loadModel(const FilePath& path) {

		// TODO
		C78_CORE_ERROR("Impl! ModelLoader");

		return Ref<Model>();
	}

}
