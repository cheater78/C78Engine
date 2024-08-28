#pragma once
#include "Asset.h"
#include "AsyncAsset.h"

namespace C78E {
	class EditorAssetManager;

	class AssetImporter {
	public:
		static Ref<Asset> importAsset(AssetHandle handle, const Asset::AssetMeta& meta, Ref<EditorAssetManager> assetManager);
		static Ref<AsyncAsset> importAssetAsync(AssetHandle handle, const Asset::AssetMeta& meta, Ref<EditorAssetManager> assetManager);
	private:
		using AssetImportFunction = std::function<Ref<Asset>(AssetHandle, const Asset::AssetMeta&, Ref<EditorAssetManager>)>;

		static const std::map<Asset::AssetType, AssetImportFunction> s_AssetImportFunctions;
	};

}
