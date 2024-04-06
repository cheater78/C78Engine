#include "C78ePCH.h"
#include "Asset.h"

namespace C78E {

	const Asset::AssetMeta Asset::c_NullAssetMeta{};

	const std::map<FilePath, Asset::AssetType> Asset::c_AssetExtensionMap = {
		{ ".pce", Asset::AssetType::Project },
		{ ".sce", Asset::AssetType::Scene },

		{ ".png", Asset::AssetType::Texture2D },
		{ ".jpg", Asset::AssetType::Texture2D },
		{ ".jpeg", Asset::AssetType::Texture2D },

		{ ".stl", Asset::AssetType::Model },
		{ ".mtl", Asset::AssetType::Material },

		{ ".glsl", Asset::AssetType::Shader },
		{ ".hlsl", Asset::AssetType::Shader }
	};

	Asset::AssetType Asset::fileToAssetType(const FilePath& filePath) {
		if (c_AssetExtensionMap.find(filePath.extension()) == c_AssetExtensionMap.end()) {
			C78_CORE_ERROR("File \"{}\" is not supported as an Asset, cannot fetch AssetType!", filePath);
			return Asset::AssetType::None;
		}
		return c_AssetExtensionMap.at(filePath.extension());
	}


	Asset::AssetType Asset::assetTypeFromString(std::string str) {
		if (str == "AssetType::Project")	return AssetType::Project;
		if (str == "AssetType::Scene")		return AssetType::Scene;

		if (str == "AssetType::Texture2D")	return AssetType::Texture2D;

		if (str == "AssetType::Shader")		return AssetType::Shader;

		if (str == "AssetType::Model")		return AssetType::Model;
		if (str == "AssetType::Mesh")		return AssetType::Mesh;
		if (str == "AssetType::Material")	return AssetType::Material;

		return AssetType::None;
	}

	std::string Asset::assetTypeToString(Asset::AssetType type) {
		switch (type) {
		case C78E::Asset::AssetType::None:			return "AssetType::None";

		case C78E::Asset::AssetType::Project:		return "AssetType::Project";
		case C78E::Asset::AssetType::Scene:			return "AssetType::Scene";

		case C78E::Asset::AssetType::Texture2D:		return "AssetType::Texture2D";

		case C78E::Asset::AssetType::Shader:		return "AssetType::Shader";

		case C78E::Asset::AssetType::Model:			return "AssetType::Model";
		case C78E::Asset::AssetType::Mesh:			return "AssetType::Mesh";
		case C78E::Asset::AssetType::Material:		return "AssetType::Material";

		default: return "AssetType::None";
		}
	}

}