#pragma once
#include <C78E/Core/UUID.h>

namespace C78E {

	using AssetHandle = UUID;
	
	class Asset {
	public:
		enum AssetType : uint8_t {
			None = 0,
			Scene = 1,

			Mesh,
			Material,

			Shader,
			Texture2D,
			CubeMap,

			Font,


		};
		static const uint8_t c_AssetTypeCount = 8;
		static const std::vector<AssetType> getAllAssetTypes(bool includeNone = true) {
			std::vector<AssetType> types;
			types.reserve(c_AssetTypeCount);
			for (uint8_t i = (includeNone) ? 0 : 1; i < c_AssetTypeCount; i++)
				types.emplace(types.begin() + i, static_cast<AssetType>(i));
			return types;
		}

		struct AssetMeta {
			AssetType type = AssetType::None;
			FilePath fileSource = "";
			std::string name = C78E_DEFAULT_ASSET_NAME;

			operator bool() const { return type != AssetType::None; }
		};

	public:
		static AssetType fileToAssetType(const FilePath& filePath);
		static std::string assetTypeToString(AssetType type);
		static AssetType assetTypeFromString(std::string str);

	public:
		static const std::map<FilePath, AssetType> c_AssetExtensionMap;
		static const AssetMeta c_NullAssetMeta;

	public:
		virtual AssetType getType() const { return AssetType::None; };
		static AssetType getClassType() { return AssetType::None; };
	public:
		AssetHandle m_AssetHandle;
	};
}

namespace std {
	_EXPORT_STD _NODISCARD inline string to_string(C78E::Asset::AssetType assetType) {
		return C78E::Asset::assetTypeToString(assetType);
	}
}