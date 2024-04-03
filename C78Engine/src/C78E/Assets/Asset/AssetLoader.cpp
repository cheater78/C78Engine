#include "C78ePCH.h"
#include "AssetLoader.h"
#include "C78E/Assets/Texture/TextureLoader.h"
#include "C78E/Assets/Scene/SceneLoader.h"
#include "C78E/Assets/Model/ModelLoader.h"
#include "C78E/Assets/Mesh/MeshLoader.h"
#include "C78E/Assets/Material/MaterialLoader.h"

namespace C78E {

	using AssetLoadFunc = std::function<Ref<Asset>(AssetHandle, const Asset::AssetMeta&)>;
	static std::map<Asset::AssetType, AssetLoadFunc> s_AssetLoadFuncs = {
		{ Asset::AssetType::Scene, SceneLoader::importScene },
		{ Asset::AssetType::Texture2D, TextureLoader::importTexture2D },
		{ Asset::AssetType::Model, ModelLoader::importModel },
		{ Asset::AssetType::Mesh, MeshLoader::importMesh },
		{ Asset::AssetType::Material, MaterialLoader::importMaterial }
	};

	Ref<Asset> AssetLoader::loadAsset(AssetHandle handle, const Asset::AssetMeta& meta) {
		if (s_AssetLoadFuncs.find(meta.type) == s_AssetLoadFuncs.end()) {
			C78_CORE_ERROR("No AssetLoader exists for Type: {}", (uint8_t)meta.type);
			return nullptr;
		}
		return s_AssetLoadFuncs.at(meta.type)(handle, meta);
	}

}
