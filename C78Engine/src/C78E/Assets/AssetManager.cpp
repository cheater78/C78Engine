#include "C78ePCH.h"
#include "AssetManager.h"

#include "Asset/AssetLoader.h"
#include <C78E/Project/Project.h>

#include <C78E/Utils/yaml-cpp/YamlUtils.h>

namespace C78E {

	/**
	* AssetManager
	*/

	

	bool AssetManager::isValid(AssetHandle handle) {
		return Project::getActive()->getAssetManager()->isValid(handle);
	}

	bool AssetManager::isLoaded(AssetHandle handle) {
		return Project::getActive()->getAssetManager()->isLoaded(handle);
	}

	Asset::AssetType AssetManager::getType(AssetHandle handle) {
		return Project::getActive()->getAssetManager()->getType(handle);
	}

	Ref<AssetManagerBase> AssetManager::getCurrentProjectsAssetManager() {
		return Project::getActive()->getAssetManager();
	}



	/**
	* EditorAssetManager
	*/

	bool EditorAssetManager::isValid(AssetHandle handle) const {
		return handle.isValid() && m_AssetRegistry.find(handle) != m_AssetRegistry.end();
	}

	bool EditorAssetManager::isLoaded(AssetHandle handle) const {
		return m_LoadedAssets.find(handle) != m_LoadedAssets.end();
	}

	Asset::AssetType EditorAssetManager::getType(AssetHandle handle) const {
		if (!isValid(handle))
			return Asset::AssetType::None;
		return m_AssetRegistry.at(handle).type;
	}

	void EditorAssetManager::loadAsset(const FilePath& filepath) {
		AssetHandle handle; // generate new handle
		Asset::AssetMeta metadata;
		metadata.fileSource = filepath;
		metadata.type = Asset::fileToAssetType(filepath);
		C78_CORE_ASSERT(metadata.type != Asset::AssetType::None);
		Ref<Asset> asset = AssetLoader::loadAsset(handle, metadata);
		if (asset) {
			asset->m_AssetHandle = handle;
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = metadata;
			serializeAssetRegistry();
		}
	}

	const Asset::AssetMeta& EditorAssetManager::getMeta(AssetHandle handle) const {
		auto it = m_AssetRegistry.find(handle);
		if (it == m_AssetRegistry.end())
			return Asset::c_NullAssetMeta;
		return it->second;
	}

	const FilePath& EditorAssetManager::getFile(AssetHandle handle) const {
		return getMeta(handle).fileSource;
	}

	Ref<Asset> EditorAssetManager::getAsset(AssetHandle handle) {
		// 1. check if handle is valid
		if (!isValid(handle))
			return nullptr;

		// 2. check if asset needs load (and if so, load)
		Ref<Asset> asset;
		if (isLoaded(handle)) {
			asset = m_LoadedAssets.at(handle);
		}
		else {
			// load asset
			const Asset::AssetMeta& metadata = getMeta(handle);
			asset = AssetLoader::loadAsset(handle, metadata);
			if (!asset) {
				// import failed
				C78_CORE_ERROR("EditorAssetManager::getAsset - loading Asset failed!");
			}
			m_LoadedAssets[handle] = asset;
		}
		// 3. return asset
		return asset;
	}

	void EditorAssetManager::serializeAssetRegistry() {
		auto path = Project::getActiveAssetRegistryPath();

		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "AssetRegistry" << YAML::Value;

			out << YAML::BeginSeq;
			for (const auto& [handle, metadata] : m_AssetRegistry) {
				out << YAML::BeginMap;
				out << YAML::Key << "Handle" << YAML::Value << handle;
				std::string filepathStr = metadata.fileSource.generic_string();
				out << YAML::Key << "FilePath" << YAML::Value << filepathStr;
				out << YAML::Key << "Type" << YAML::Value << Asset::assetTypeToString(metadata.type);
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(path);
		fout << out.c_str();
	}

	bool EditorAssetManager::deserializeAssetRegistry() {
		auto path = Project::getActiveAssetRegistryPath();
		YAML::Node data;
		try {
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e) {
			C78_CORE_ERROR("Failed to load project file '{0}'\n     {1}", path, e.what());
			return false;
		}

		auto rootNode = data["AssetRegistry"];
		if (!rootNode)
			return false;

		for (const auto& node : rootNode) {
			AssetHandle handle = node["Handle"].as<uint64_t>();
			auto& metadata = m_AssetRegistry[handle];
			metadata.fileSource = node["FilePath"].as<std::string>();
			metadata.type = Asset::assetTypeFromString(node["Type"].as<std::string>());
		}

		return true;
	}

	AssetHandle EditorAssetManager::addAsset(Asset::AssetMeta meta, Ref<Asset> asset) {
		AssetHandle handle; // generate new handle
		C78_CORE_ASSERT(meta.type != Asset::AssetType::None);
		if (asset) {
			asset->m_AssetHandle = handle;
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = meta;
			serializeAssetRegistry();
			return handle;
		}
		return 0;
	}

}