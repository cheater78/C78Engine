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
		map[Asset::Type::Scene] = SceneImporter::importScene;
		map[Asset::Type::Shader] = ShaderImporter::importShader;
		map[Asset::Type::Texture2D] = TextureImporter::importTexture2D;
		map[Asset::Type::CubeMap] = TextureImporter::importCubeMap;
		map[Asset::Type::Font] = FontImporter::importFont;
		return map;
	}
	

	Ref<Asset> AssetImporter::importAsset(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle) {
		C78E_CORE_VALIDATE(AssetImporter::AssetImport::assetImportFunctions.contains(meta->type), return nullptr, "No AssetLoader exists for Type: {}", Asset::Type::assetTypeToString(meta->type));
		return AssetImporter::AssetImport::assetImportFunctions.at(meta->type)(assetDirectory, meta, handle);
	}
	
}
