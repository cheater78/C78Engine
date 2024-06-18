#include "C78ePCH.h"
#include "AssetImporter.h"
#include "C78E/Assets/Texture/TextureLoader.h"
#include "C78E/Assets/Scene/SceneLoader.h"
#include "C78E/Assets/Model/ModelLoader.h"
#include "C78E/Assets/Mesh/MeshLoader.h"
#include "C78E/Assets/Material/MaterialLoader.h"

namespace C78E {

	const std::map<Asset::AssetType, AssetImporter::AssetImportFunction> AssetImporter::s_AssetImportFunctions = {
		{ Asset::AssetType::Scene, SceneLoader::importScene },
		{ Asset::AssetType::Texture2D, TextureLoader::importTexture2D },
		{ Asset::AssetType::Model, ModelLoader::importModel },
		{ Asset::AssetType::Mesh, MeshLoader::importMesh },
		{ Asset::AssetType::Material, MaterialLoader::importMaterial }
	};

	Ref<Asset> AssetImporter::importAsset(AssetHandle handle, const Asset::AssetMeta& meta) {
		if (s_AssetImportFunctions.find(meta.type) == s_AssetImportFunctions.end()) {
			C78_CORE_ERROR("No AssetLoader exists for Type: {}", Asset::assetTypeToString(meta.type));
			return nullptr;
		}
		return s_AssetImportFunctions.at(meta.type)(handle, meta);
	}

	Ref<AsyncAsset> AssetImporter::importAssetAsync(AssetHandle handle, const Asset::AssetMeta& meta) {
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
			assetRef->m_Asset = s_AssetImportFunctions.at(meta.type)(handle, meta);
			assetRef->m_Loaded = true;
		}

		//return Ref
		return assetRef;
	}

}
