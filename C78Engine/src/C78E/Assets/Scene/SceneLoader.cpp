#include "C78ePCH.h"
#include "SceneLoader.h"

#include <C78E/Project/Project.h>
#include <C78E/Scene/SceneSerializer.h>

namespace C78E {

	Ref<Scene> SceneLoader::importScene(AssetHandle handle, const Asset::AssetMeta& meta) {
		return loadScene(Project::getActiveAssetDirectory() / meta.fileSource);
	}

	Ref<Scene> SceneLoader::loadScene(const FilePath& path) {
		Ref<Scene> scene = createRef<Scene>();
		SceneSerializer serializer(scene);
		serializer.deserialize(path);
		return scene;
	}

	void SceneLoader::SaveScene(Ref<Scene> scene, const FilePath& path) {
		SceneSerializer serializer(scene);
		serializer.serialize(Project::getActiveAssetDirectory() / path);
	}

}