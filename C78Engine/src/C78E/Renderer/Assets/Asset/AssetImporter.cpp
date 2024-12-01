#include "C78EPCH.h"
#include "AssetImporter.h"

#include <C78E/Renderer/Assets/AssetManager.h>

#include <C78E/Renderer/Assets/Texture/TextureImporter.h>
#include <C78E/Renderer/Assets/Scene/SceneSerializer.h>
#include <C78E/Renderer/Assets/Shader/ShaderImporter.h>
#include <C78E/Renderer/Assets/Font/FontImporter.h>

namespace C78E {

	const AssetImporter::AssetImport::AssetImportFunctionMap AssetImporter::AssetImport::assetImportFunctions = AssetImporter::AssetImport::createAssetImportFunctionMap();
	AssetImporter::AssetImport::AssetImportFunctionMap AssetImporter::AssetImport::createAssetImportFunctionMap() {
		AssetImportFunctionMap map;
		map[Asset::AssetType::Scene] = SceneImporter::importScene;
		map[Asset::AssetType::Shader] = ShaderImporter::importShader;
		map[Asset::AssetType::Texture2D] = TextureImporter::importTexture2D;
		map[Asset::AssetType::CubeMap] = TextureImporter::importCubeMap;
		map[Asset::AssetType::Font] = FontImporter::importFont;
		return map;
	}
	

	Ref<Asset> AssetImporter::importAsset(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle) {
		if (!AssetImporter::AssetImport::assetImportFunctions.contains(meta.type)) {
			C78E_CORE_ERROR("No AssetLoader exists for Type: {}", Asset::assetTypeToString(meta.type));
			return nullptr;
		}
		return AssetImporter::AssetImport::assetImportFunctions.at(meta.type)(assetDirectory, meta, handle);
	}
	
}
