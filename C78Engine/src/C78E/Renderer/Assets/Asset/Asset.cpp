#include "C78EPCH.h"
#include "Asset.h"

#include <C78E/Renderer/Assets/Texture/TextureImporter.h>
#include <C78E/Renderer/Assets/Scene/SceneSerializer.h>
#include <C78E/Renderer/Assets/Shader/ShaderImporter.h>
#include <C78E/Renderer/Assets/Mesh/MeshImporter.h>
#include <C78E/Renderer/Assets/Font/FontImporter.h>

namespace C78E {

	const Asset::FileMap Asset::c_FileMap = {
		{
			{ C78E_FILE_EXT_SCENE, Asset::Type::Scene },

			{ C78E_FILE_EXT_IMAGE_PNG, Asset::Type::Texture },
			{ C78E_FILE_EXT_IMAGE_JPG, Asset::Type::Texture },
			{ C78E_FILE_EXT_IMAGE_JPEG, Asset::Type::Texture },

			{ C78E_FILE_EXT_WAFEFRONT_MATERIAL, Asset::Type::Material },

			{ C78E_FILE_EXT_SHADER_GLSL, Asset::Type::Shader },
			{ C78E_FILE_EXT_SHADER_HLSL, Asset::Type::Shader },

			{ C78E_FILE_EXT_FONT_TTF, Asset::Type::Font },
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
		case C78E::Asset::Type::Texture:	return "Asset::Type::Texture";
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
		if (suffix == "Texture")	return Type::Texture;
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

	bool Asset::Group::merge(const Group& other) {
		bool disjunct = true;
		for (auto [asset, meta] : other) {
			auto it = find(asset);
			if (it != end())
				disjunct = false;
			else emplace(asset, meta);
		}
	}

	const AssetImporter::AssetImport::AssetImportFunctionMap AssetImporter::AssetImport::assetImportFunctions = AssetImporter::AssetImport::createAssetImportFunctionMap();
	AssetImporter::AssetImport::AssetImportFunctionMap AssetImporter::AssetImport::createAssetImportFunctionMap() {
		AssetImportFunctionMap map;
		map[Asset::Type::Scene] = SceneImporter::importScene;
		map[Asset::Type::Mesh] = MeshImporter::importMesh;
		map[Asset::Type::Shader] = ShaderImporter::importShader;
		map[Asset::Type::Texture] = TextureImporter::importTexture2D;
		map[Asset::Type::Font] = FontImporter::importFont;
		return map;
	}


	inline AssetImporter::AssetImporter(const FilePath& assetDirectory)
		: m_AssetDirectory(assetDirectory) {
	}

	AssetImporter::~AssetImporter() { }

	Ref<Asset::Group> AssetImporter::import(Ref<Asset::Group> assets) {
		C78E_CORE_VALIDATE(assets, return nullptr, "AssetImporter::import: assets is null!");
		for (auto& [asset, meta] : *assets) {

		}
		return assets;
	}

	Ref<Asset::Group> AssetImporter::importAsset(AssetHandle handle, Ref<Asset::Meta> meta) {
		C78E_CORE_VALIDATE(AssetImporter::AssetImport::assetImportFunctions.contains(meta->type), return nullptr, "No AssetLoader exists for Type: {}", Asset::Type::assetTypeToString(meta->type));
		return AssetImporter::AssetImport::assetImportFunctions.at(meta->type)(m_AssetDirectory, meta, handle);
	}

}