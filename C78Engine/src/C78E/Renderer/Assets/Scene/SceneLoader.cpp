#include "C78ePCH.h"
#include "SceneLoader.h"

#include <C78E/Project/Project.h>
#include <C78E/Renderer/Assets/Scene/SceneSerializer.h>

namespace C78E {

	Ref<Scene> SceneLoader::importScene(AssetHandle handle, const Asset::AssetMeta& meta) {
		Ref<Scene> scene = createRef<Scene>();
		SceneSerializer serializer(scene, meta);
		C78_CORE_ERROR("SceneLoader::importScene: DISABLED!");
		//serializer.deserialize(Project::getActiveAssetDirectory() / meta.fileSource);
		return scene;
	}

	Ref<Scene> SceneLoader::loadScene(const FilePath& path) {

		// No such thing?!

		Ref<Scene> scene = createRef<Scene>();
		Asset::AssetMeta meta;
		SceneSerializer serializer(scene, meta);
		serializer.deserialize(path);
		return scene;
	}

	void SceneLoader::saveScene(Ref<Scene> scene, const Asset::AssetMeta& meta) {
		SceneSerializer serializer(scene, meta);
		serializer.serialize();
	}

}