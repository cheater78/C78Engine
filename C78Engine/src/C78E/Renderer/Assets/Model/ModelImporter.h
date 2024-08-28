#pragma once
#include "Model.h"

namespace C78E {
	class EditorAssetManager;

	class ModelImporter {
	public:
		static Ref<Model> importModel(AssetHandle handle, const Asset::AssetMeta& meta, Ref<EditorAssetManager> assetManager);

	private:
		static Ref<Model> loadWavefrontModel(AssetHandle handle, const Asset::AssetMeta& meta, Ref<EditorAssetManager> assetManager);
	};

}