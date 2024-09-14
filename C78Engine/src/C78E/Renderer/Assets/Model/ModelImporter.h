#pragma once
#include "Model.h"
#include "ModelComposite.h"

namespace C78E {

	class ModelImporter {
	public:
		static Ref<Model> importModel(AssetHandle handle, const Asset::AssetMeta& meta);
		static Ref<ModelComposite> importModelComposite(AssetHandle handle, const Asset::AssetMeta& meta);

	private:
		static Ref<ModelComposite> loadWavefrontModel(AssetHandle handle, const Asset::AssetMeta& meta, const uint32_t maxParts = 0);
	};

}