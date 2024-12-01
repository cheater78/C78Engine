#include "C78EPCH.h"
#include "Asset.h"

namespace C78E {

	const Asset::AssetMeta Asset::c_NullAssetMeta{};

	const std::map<FilePath, Asset::AssetType> Asset::c_AssetExtensionMap = {
		{ ".sce", Asset::AssetType::Scene },

		{ ".png", Asset::AssetType::Texture2D },
		{ ".jpg", Asset::AssetType::Texture2D },
		{ ".jpeg", Asset::AssetType::Texture2D },

		{ ".mtl", Asset::AssetType::Material },

		{ ".glsl", Asset::AssetType::Shader },
		{ ".hlsl", Asset::AssetType::Shader },

		{ ".ttf", Asset::AssetType::Font },
	};

	/**
	 * Determines the Asset::AssetType of an arbituary file based on its extension,
	 * using the Asset::c_AssetExtensionMap.
	 * 
	 * \param filePath the C78E::FilePath of the file in question
	 * \return the Asset::AssetType of the file
	 */
	Asset::AssetType Asset::fileToAssetType(const FilePath& filePath) {
		if (c_AssetExtensionMap.find(filePath.extension()) == c_AssetExtensionMap.end()) {
			C78E_CORE_ERROR("File \"{}\" is not supported as an Asset, cannot fetch AssetType!", filePath);
			return Asset::AssetType::None;
		}
		return c_AssetExtensionMap.at(filePath.extension());
	}

	/**
	 * Serializes a Asset::AssetType to a std::string.
	 * 
	 * \param type the Asset::AssetType to serialize
	 * \return the serialized Asset::AssetType as std::string
	 */
	std::string Asset::assetTypeToString(Asset::AssetType type) {
		switch (type) {
		case C78E::Asset::AssetType::None:			return "AssetType::None";
		
		case C78E::Asset::AssetType::Scene:			return "AssetType::Scene";

		case C78E::Asset::AssetType::Mesh:			return "AssetType::Mesh";
		case C78E::Asset::AssetType::Material:		return "AssetType::Material";

		case C78E::Asset::AssetType::Shader:		return "AssetType::Shader";

		case C78E::Asset::AssetType::Texture2D:		return "AssetType::Texture2D";
		case C78E::Asset::AssetType::CubeMap:		return "AssetType::CubeMap";

		case C78E::Asset::AssetType::Font:		return "AssetType::Font";

		default: return "AssetType::None";
		}
	}

	/**
	 * Deserializes a std::string to Asset::AssetType.
	 * 
	 * \param str the string containing the serialized Asset::AssetType
	 * \return the deserialized Asset::AssetType
	 */
	Asset::AssetType Asset::assetTypeFromString(std::string str) {
		if (str == "AssetType::Scene")		return AssetType::Scene;

		if (str == "AssetType::Mesh")		return AssetType::Mesh;
		if (str == "AssetType::Material")	return AssetType::Material;

		if (str == "AssetType::Shader")		return AssetType::Shader;

		if (str == "AssetType::Texture2D")	return AssetType::Texture2D;
		if (str == "AssetType::CubeMap")	return AssetType::CubeMap;

		if (str == "AssetType::Font")	return AssetType::Font;

		return AssetType::None;
	}

}