#pragma once
#include "Asset.h"

namespace C78E {

	class AssetImporter {
	public:
		static Ref<Asset> importAsset(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle);
	private:
		struct AssetImport {
		public:
			using AssetImportFunction = std::function<Ref<Asset>(const FilePath&, const Asset::AssetMeta&, AssetHandle)>;
			using AssetImportFunctionMap = std::map<Asset::AssetType, AssetImportFunction>;
			static const AssetImportFunctionMap assetImportFunctions;

			static AssetImportFunctionMap createAssetImportFunctionMap();
		};

	};

}
