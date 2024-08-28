#include "C78ePCH.h"
#include "AssetImporter.h"

#include <C78E/Renderer/Assets/AssetManager.h>

#include <C78E/Renderer/Assets/Texture/TextureImporter.h>
#include <C78E/Renderer/Assets/Scene/SceneImporter.h>
#include <C78E/Renderer/Assets/Model/ModelImporter.h>
#include <C78E/Renderer/Assets/Shader/ShaderImporter.h>

namespace C78E {

	const std::map<Asset::AssetType, AssetImporter::AssetImportFunction> AssetImporter::s_AssetImportFunctions = {
		{ Asset::AssetType::Scene, SceneImporter::importScene },
		{ Asset::AssetType::Texture2D, TextureImporter::importTexture2D },
		{ Asset::AssetType::Model, ModelImporter::importModel },
		{ Asset::AssetType::Shader, ShaderImporter::importShader },
	};

	Ref<Asset> AssetImporter::importAsset(AssetHandle handle, const Asset::AssetMeta& meta, Ref<EditorAssetManager> assetManager) {
		if (s_AssetImportFunctions.find(meta.type) == s_AssetImportFunctions.end()) {
			C78_CORE_ERROR("No AssetLoader exists for Type: {}", Asset::assetTypeToString(meta.type));
			return nullptr;
		}
		return s_AssetImportFunctions.at(meta.type)(handle, meta, assetManager);
	}

	Ref<AsyncAsset> AssetImporter::importAssetAsync(AssetHandle handle, const Asset::AssetMeta& meta, Ref<EditorAssetManager> assetManager) {
		if (s_AssetImportFunctions.find(meta.type) == s_AssetImportFunctions.end()) {
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
			assetRef->m_Asset = s_AssetImportFunctions.at(meta.type)(handle, meta, assetManager);
			assetRef->m_Loaded = true;
		}

		//return Ref
		return assetRef;
	}

}
