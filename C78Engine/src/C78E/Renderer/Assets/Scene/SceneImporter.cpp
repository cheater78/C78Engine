#include "C78ePCH.h"
#include "SceneImporter.h"

#include <C78E/Project/Project.h>
#include <C78E/Renderer/Assets/Scene/SceneSerializer.h>

#include <C78E/Renderer/Assets/AssetManager.h>

namespace C78E {

	Ref<Scene> SceneImporter::importScene(AssetHandle handle, const Asset::AssetMeta& meta, Ref<EditorAssetManager> assetManager) {
		Ref<Scene> scene = createRef<Scene>();
		SceneSerializer serializer(scene, meta);
		serializer.deserialize(meta.fileSource);
		return scene;
	}
}