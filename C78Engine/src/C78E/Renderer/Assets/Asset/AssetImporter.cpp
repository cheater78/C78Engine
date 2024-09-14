#include "C78ePCH.h"
#include "AssetImporter.h"

#include <C78E/Renderer/Assets/AssetManager.h>

#include <C78E/Renderer/Assets/Texture/TextureImporter.h>
#include <C78E/Renderer/Assets/Texture/CubeMapImporter.h>
#include <C78E/Renderer/Assets/Scene/SceneImporter.h>
#include <C78E/Renderer/Assets/Model/ModelImporter.h>
#include <C78E/Renderer/Assets/Shader/ShaderImporter.h>
#include <C78E/Renderer/Assets/Font/FontImporter.h>

namespace C78E {

	const AssetImporter::AssetImport::AssetImportFunctionMap AssetImporter::AssetImport::assetImportFunctions = AssetImporter::AssetImport::createAssetImportFunctionMap();
	AssetImporter::AssetImport::AssetImportFunctionMap AssetImporter::AssetImport::createAssetImportFunctionMap() {
		AssetImportFunctionMap map;
		map[Asset::AssetType::Scene] = SceneImporter::importScene;
		map[Asset::AssetType::Model] = ModelImporter::importModel;
		map[Asset::AssetType::Shader] = ShaderImporter::importShader;
		map[Asset::AssetType::Texture2D] = TextureImporter::importTexture2D;
		map[Asset::AssetType::CubeMap] = CubeMapImporter::importCubeMap;
		map[Asset::AssetType::Font] = FontImporter::importFont;
		return map;
	}
	

	Ref<Asset> AssetImporter::importAsset(AssetHandle handle, const Asset::AssetMeta& meta) {
		if (AssetImporter::AssetImport::assetImportFunctions.find(meta.type) == AssetImporter::AssetImport::assetImportFunctions.end()) {
			C78_CORE_ERROR("No AssetLoader exists for Type: {}", Asset::assetTypeToString(meta.type));
			return nullptr;
		}
		return AssetImporter::AssetImport::assetImportFunctions.at(meta.type)(handle, meta);
	}

	Ref<AsyncAsset> AssetImporter::importAssetAsync(AssetHandle handle, const Asset::AssetMeta& meta) {
		if (AssetImporter::AssetImport::assetImportFunctions.find(meta.type) == AssetImporter::AssetImport::assetImportFunctions.end()) {
			C78_CORE_ERROR("No AssetLoader exists for Type: {}", Asset::assetTypeToString(meta.type));
			return nullptr;
		}
		//create empty AsyncAsset
		Ref<AsyncAsset> assetRef = createRef<AsyncAsset>();
		//imidiatly set the already loaded default Asset and set Loaded to false
		assetRef->m_DefaultAsset = nullptr;
		assetRef->m_Loaded = false;

		// get new Thread
		//std::thread th;
		{
			assetRef->m_Asset = AssetImporter::AssetImport::assetImportFunctions.at(meta.type)(handle, meta);
			assetRef->m_Loaded = true;
		}

		//return Ref
		return assetRef;
	}

	

}
