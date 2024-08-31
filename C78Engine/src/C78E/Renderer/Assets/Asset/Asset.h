#pragma once
#include <C78E/Core/UUID.h>

namespace C78E {

	using AssetHandle = UUID;
	
	class Asset {
	public:
		enum AssetType : uint8_t {
			None = 0,

			Scene = 1,

			Model,
			Mesh,
			Material,

			Shader,
			Texture2D,

		};
		static const uint8_t c_AssetTypeCount = 7;
		static const std::vector<AssetType> getAllAssetTypes() {
			std::vector<AssetType> types;
			types.reserve(c_AssetTypeCount);
			for (uint8_t i = 0; i < c_AssetTypeCount; i++)
				types.emplace(types.begin() + i, static_cast<AssetType>(i));
			return types;
		}

		struct AssetMeta {
			AssetType type = AssetType::None;
			FilePath fileSource = "/";
			std::string name = "Unnamed Asset"; //TODO: centrl

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