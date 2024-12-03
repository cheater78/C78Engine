#pragma once
#include "Asset.h"

namespace C78E {

	class AssetImporter {
	public:
		static Ref<Asset> importAsset(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle);
	private:
		struct AssetImport {
		public:
			using AssetImportFunction = std::function<Ref<Asset>(const FilePath&, Ref<Asset::Meta>, AssetHandle)>;
			using AssetImportFunctionMap = std::map<Asset::Type, AssetImportFunction>;
			static const AssetImportFunctionMap assetImportFunctions;

			static AssetImportFunctionMap createAssetImportFunctionMap();
		};

	};

}
