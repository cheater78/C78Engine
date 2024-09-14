#pragma once
#include "Asset.h"
#include "AsyncAsset.h"

namespace C78E {

	class AssetImporter {
	public:
		static Ref<Asset> importAsset(AssetHandle handle, const Asset::AssetMeta& meta);
		static Ref<AsyncAsset> importAssetAsync(AssetHandle handle, const Asset::AssetMeta& meta);
	private:
		struct AssetImport {
		public:
			using AssetImportFunction = std::function<Ref<Asset>(AssetHandle, const Asset::AssetMeta&)>;
			using AssetImportFunctionMap = std::map<Asset::AssetType, AssetImportFunction>;
			static const AssetImportFunctionMap assetImportFunctions;

			static AssetImportFunctionMap createAssetImportFunctionMap();
		};

	};

}
