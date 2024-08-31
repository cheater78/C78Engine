#pragma once
#include "Asset.h"
#include "AsyncAsset.h"

namespace C78E {
	class EditorAssetManager;

	class AssetImporter {
	public:
		static Ref<Asset> importAsset(AssetHandle handle, const Asset::AssetMeta& meta);
		static Ref<AsyncAsset> importAssetAsync(AssetHandle handle, const Asset::AssetMeta& meta);
	private:
		using AssetImportFunction = std::function<Ref<Asset>(AssetHandle, const Asset::AssetMeta&)>;

		static const std::map<Asset::AssetType, AssetImportFunction> s_AssetImportFunctions;
	};

}
