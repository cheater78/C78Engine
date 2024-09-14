#include "C78ePCH.h"
#include "SceneImporter.h"

#include <C78E/Renderer/Assets/Scene/SceneSerializer.h>

namespace C78E {

	Ref<Scene> SceneImporter::importScene(AssetHandle handle, const Asset::AssetMeta& meta) {
		Ref<Scene> scene = createRef<Scene>();
		SceneSerializer serializer(scene, meta);
		serializer.deserialize(meta.fileSource);
		return scene;
	}
}