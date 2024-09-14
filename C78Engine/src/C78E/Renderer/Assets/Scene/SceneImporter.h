#pragma once

#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/Assets/Scene/Scene.h>

namespace C78E {

	class SceneImporter {
	public:
		static Ref<Scene> importScene(AssetHandle handle, const Asset::AssetMeta& meta);
	};

}