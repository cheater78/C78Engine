#pragma once

#include <C78E/Assets/Asset/Asset.h>
#include <C78E/Assets/Scene/Scene.h>

namespace C78E {

	class SceneLoader {
	public:
		// Load from filepath
		static Ref<Scene> loadScene(const FilePath& path);

		// AssetMetadata filepath is relative to project asset directory
		static Ref<Scene> importScene(AssetHandle handle, const Asset::AssetMeta& meta);

		static void saveScene(Ref<Scene> scene, const Asset::AssetMeta& meta);
	};

}