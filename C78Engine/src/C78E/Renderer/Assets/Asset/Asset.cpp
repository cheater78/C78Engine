#include "C78EPCH.h"
#include "Asset.h"

namespace C78E {

	const Asset::FileMap Asset::c_FileMap = {
		{
			{ ".sce", Asset::Type::Scene },

			{ ".png", Asset::Type::Texture2D },
			{ ".jpg", Asset::Type::Texture2D },
			{ ".jpeg", Asset::Type::Texture2D },
			{ ".png", Asset::Type::CubeMap },
			{ ".jpg", Asset::Type::CubeMap },
			{ ".jpeg", Asset::Type::CubeMap },

			{ ".mtl", Asset::Type::Material },

			{ ".glsl", Asset::Type::Shader },
			{ ".hlsl", Asset::Type::Shader },

			{ ".ttf", Asset::Type::Font },
		}
	};

	/**
	 * Determines the Asset::Type of an arbituary file based on its extension,
	 * using the Asset::c_FileMap.
	 * 
	 * \param filePath the C78E::FilePath of the file in question
	 * \return the Asset::Type of the file
	 */
	Asset::Type Asset::Type::typeFromFile(const FilePath& filePath) {
		return c_FileMap.getTypeFromExtension(filePath);
	}

	/**
	 * Determines the Asset::Types of an arbituary file based on its extension,
	 * using the Asset::c_FileMap.
	 * 
	 * \param filePath the C78E::FilePath of the file in question
	 * \return a set of Asset::Types of the file
	 */
	std::set<Asset::Type> Asset::Type::typesFromFile(const FilePath& filePath) {
		return c_FileMap.getTypesFromExtension(filePath);
	}

	/**
	 * Serializes a Asset::Type to a std::string.
	 * 
	 * \param type the Asset::Type to serialize
	 * \return the serialized Asset::Type as std::string
	 */
	std::string Asset::Type::assetTypeToString(Asset::Type type) {
		switch ((uint8_t)type) {
		case C78E::Asset::Type::None:		return "Asset::Type::None";
		case C78E::Asset::Type::Scene:		return "Asset::Type::Scene";
		case C78E::Asset::Type::Mesh:		return "Asset::Type::Mesh";
		case C78E::Asset::Type::Material:	return "Asset::Type::Material";
		case C78E::Asset::Type::Shader:		return "Asset::Type::Shader";
		case C78E::Asset::Type::Texture2D:	return "Asset::Type::Texture2D";
		case C78E::Asset::Type::CubeMap:	return "Asset::Type::CubeMap";
		case C78E::Asset::Type::Font:		return "Asset::Type::Font";

		default: return "Asset::Type::None";
		}
	}

	/**
	 * Deserializes a std::string to Asset::Type.
	 * 
	 * \param str the string containing the serialized Asset::Type
	 * \return the deserialized Asset::Type
	 */
	Asset::Type Asset::Type::assetTypeFromString(const std::string& typeString) {
		const std::string prefix = "Asset::Type::";
		if(typeString.substr(0, prefix.size()) != prefix) return Type::None;
		const std::string suffix = typeString.substr(prefix.size());

		if (suffix == "Scene")		return Type::Scene;
		if (suffix == "Mesh")		return Type::Mesh;
		if (suffix == "Material")	return Type::Material;
		if (suffix == "Shader")		return Type::Shader;
		if (suffix == "Texture2D")	return Type::Texture2D;
		if (suffix == "CubeMap")	return Type::CubeMap;
		if (suffix == "Font")		return Type::Font;

		return Type::None;
	}


	std::set<Asset::Type> Asset::FileMap::getTypesFromExtension(const FilePath& file) const {
		if(!file.has_extension()) return { Type::None };
		const std::string extension = file.extension().string();

		std::set<Asset::Type> types = { };
		for(auto& [ext, type] : extensionMap)
			if(ext == extension)
				types.insert(type);
		
		return ((types.size()) ? types : std::set<Asset::Type>{ Type::None });
	}

	Asset::Type Asset::FileMap::getTypeFromExtension(const FilePath& file) const {
		if(!file.has_extension()) return Type::None;
		const std::string extension = file.extension().string();

		for(auto& [ext, type] : extensionMap)
			if(ext == extension)
				return type;
		
		return Type::None;
	}
}