#pragma once

#include <C78E/Assets/Asset/Asset.h>
#include <C78E/Scene/Scene.h>

namespace C78E {

	class SceneLoader {
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Scene> importScene(AssetHandle handle, const Asset::AssetMeta& meta);

		// Load from filepath
		static Ref<Scene> loadScene(const FilePath& path);

		static void SaveScene(Ref<Scene> scene, const FilePath& path);
	};

}