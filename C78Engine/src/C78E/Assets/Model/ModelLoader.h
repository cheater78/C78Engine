#pragma once
#include "Model.h"

namespace C78E {

	class ModelLoader {
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Model> importModel(AssetHandle handle, const Asset::AssetMeta& meta);

		// Load from filepath
		static Ref<Model> loadModel(const FilePath& path);
	private:

	};

}