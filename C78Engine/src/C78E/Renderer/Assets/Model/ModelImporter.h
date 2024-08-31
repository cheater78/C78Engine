#pragma once
#include "Model.h"

namespace C78E {
	class EditorAssetManager;

	class ModelImporter {
	public:
		static Ref<Model> importModel(AssetHandle handle, const Asset::AssetMeta& meta);

	private:
		static Ref<Model> loadWavefrontModel(AssetHandle handle, const Asset::AssetMeta& meta);
	};

}