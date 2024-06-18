#pragma once
#include <C78E/Core/UUID.h>

namespace C78E {

	using AssetHandle = UUID;
	
	class Asset {
	public:
		enum AssetType : uint8_t {
			None = 0,

			Project = 1,
			Scene = 2,

			Texture2D,

			Shader,

			Model,
			Mesh,
			Material

		};

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
	public:
		AssetHandle m_AssetHandle;
	};

}