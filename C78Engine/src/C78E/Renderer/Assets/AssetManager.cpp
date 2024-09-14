#include "C78ePCH.h"
#include "AssetManager.h"

#include "Asset/AssetImporter.h"

#include <C78E/Utils/Yaml/YamlUtils.h>

#include <C78E/Renderer/API/Texture.h>
#include <C78E/Renderer/API/Shader.h>

namespace C78E {

	/*
	* EditorAssetManager
	*/

	EditorAssetManager::EditorAssetManager() {
		Default::createDefaultAssets(this);
	}

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

	AssetHandle EditorAssetManager::importAsset(const FilePath& filepath, Asset::AssetMeta meta, AssetHandle handle) {
		if (!handle.isValid())
			handle = UUID();
		if (meta.type == Asset::AssetType::None) {
			meta.type = Asset::fileToAssetType(filepath);
			if (meta.type == Asset::AssetType::None) {
				C78_CORE_ERROR("EditorAssetManager::importAsset: '{}' files are not supported yet!", filepath.extension().string());
				return AssetHandle::invalid();
			}
		}

		Ref<Asset> asset = AssetImporter::importAsset(handle, meta);
		if (asset) {
			asset->m_AssetHandle = handle;
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = meta;
			//TODO: if autosave... then here -> EditorAssetManager do
			return asset->m_AssetHandle;
		}
		C78_CORE_ERROR("EditorAssetManager::importAsset: '{}' failed to import!", filepath.string());
		return AssetHandle::invalid();
	}

	bool EditorAssetManager::removeAsset(AssetHandle handle, bool fromDisk) {
		if (!isValid(handle)) {
			C78_CORE_ERROR("EditorAssetManager::removeAsset: Assethandle is null!");
			return false;
		}
		if (fromDisk) {
			auto& assetMeta = getMeta(handle);
			C78_CORE_ERROR("EditorAssetManager::removeAsset: TODO - Cannot remove from Disk yet!");
		}
		m_AssetRegistry.erase(handle);
		m_LoadedAssets.erase(handle);
		return true;
	}

	bool EditorAssetManager::reloadAsset(AssetHandle handle) {
		if (!handle.isValid()) return false;

		auto defaultAssetIndex = m_DefaultAssets.find(handle);
		if (defaultAssetIndex != m_DefaultAssets.end()) {
			Default::createDefaultAssets(this);
			return defaultAssetIndex->second != nullptr;
		}

		bool isKnown = getAssetRegistry().find(handle) != getAssetRegistry().end();
		if (!isKnown) return false;
		Asset::AssetMeta meta = getMeta(handle);
		if (meta == Asset::c_NullAssetMeta) return false;

		if (!isLoaded(handle)) {
			Ref<Asset> asset = getAsset(handle);
			return asset != nullptr;
		}
		m_LoadedAssets[handle] = AssetImporter::importAsset(handle, meta);
		return m_LoadedAssets[handle] != nullptr;
	}

	Asset::AssetMeta& EditorAssetManager::getMeta(AssetHandle handle) {
		auto it = m_AssetRegistry.find(handle);
		C78_CORE_ASSERT(it != m_AssetRegistry.end(), "EditorAssetManager::getMeta: AssetHandle not found!");
		return it->second;
	}

	const FilePath& EditorAssetManager::getFile(AssetHandle handle) {
		return getMeta(handle).fileSource;
	}

	Ref<Asset> EditorAssetManager::getAsset(AssetHandle handle) {
		if (handle.isValid() && m_DefaultAssets.find(handle) != m_DefaultAssets.end()) return m_DefaultAssets.at(handle);

		if (!isValid(handle))
			return nullptr;
		
		if (isLoaded(handle)) {
			return m_LoadedAssets.at(handle);
		} else {
			Ref<Asset> asset;
			const Asset::AssetMeta& metadata = getMeta(handle);
			asset = AssetImporter::importAsset(handle, metadata);
			if (!asset) {
				// import failed
				C78_CORE_ERROR("EditorAssetManager::getAsset - loading Asset failed!");
			}
			asset->m_AssetHandle = handle;
			m_LoadedAssets[handle] = asset;
			return asset;
		}
	}

	void EditorAssetManager::exportAssetRegistry(const FilePath& assetRegistryPath) {
		FileSystem::createDirectoryIfNotPresent(assetRegistryPath.parent_path());
		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "AssetRegistry" << YAML::Value;

			out << YAML::BeginSeq;
			for (const auto& [handle, metadata] : m_AssetRegistry) {
				out << YAML::BeginMap;
				out << YAML::Key << "Handle" << YAML::Value << handle;
				std::string filepathStr = metadata.fileSource.generic_string();
				out << YAML::Key << "Name" << YAML::Value << metadata.name;
				out << YAML::Key << "FilePath" << YAML::Value << filepathStr;
				out << YAML::Key << "Type" << YAML::Value << Asset::assetTypeToString(metadata.type);
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(assetRegistryPath);
		fout << out.c_str();
	}

	bool EditorAssetManager::importAssetRegistry(const FilePath& assetRegistryPath) {
		C78_CORE_ASSERT(FileSystem::exists(assetRegistryPath), "EditorAssetManager::deserializeAssetRegistry: AssetRegistryPath does not exist!");
		YAML::Node data;
		try {
			data = YAML::LoadFile(assetRegistryPath.string());
		}
		catch (YAML::ParserException e) {
			C78_CORE_ERROR("Failed to load project file '{0}'\n     {1}", assetRegistryPath, e.what());
			return false;
		}

		auto rootNode = data["AssetRegistry"];
		if (!rootNode) {
			C78_CORE_ERROR("EditorAssetManager::importAssetRegistry: Root Node not found!");
			return false;
		}

		for (const auto& node : rootNode) {
			AssetHandle handle = node["Handle"].as<AssetHandle>();
			auto& metadata = m_AssetRegistry[handle];
			metadata.name = node["Name"].as<std::string>();
			metadata.fileSource = node["FilePath"].as<std::string>();
			metadata.type = Asset::assetTypeFromString(node["Type"].as<std::string>());
		}
		return true;
	}

	AssetHandle EditorAssetManager::addAsset(Asset::AssetMeta meta, Ref<Asset> asset) {
		if (meta.type == Asset::AssetType::None) {
			C78_CORE_ERROR("EditorAssetManager::addAsset: given AssetMeta.type cannot be None!");
			return AssetHandle::invalid();
		}
		if (asset) {
			AssetHandle handle; // generate new handle
			asset->m_AssetHandle = handle;
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = meta;
			//TODO: if autosave... then here -> EditorAssetManager do
			return handle;
		} else {
			C78_CORE_ERROR("EditorAssetManager::addAsset: given asset is a nullptr!");
			return AssetHandle::invalid();
		}
		
	}

	const AssetHandle EditorAssetManager::Default::Texture2D_White = EditorAssetManager::Default::getAssetHandle(1);
	const AssetHandle EditorAssetManager::Default::Shader_SpriteRenderComponent = EditorAssetManager::Default::getAssetHandle(2);
	const AssetHandle EditorAssetManager::Default::Shader_TextCompoent = EditorAssetManager::Default::getAssetHandle(3);
	const AssetHandle EditorAssetManager::Default::Shader_ModelCompoent = EditorAssetManager::Default::getAssetHandle(4);

	const AssetHandle EditorAssetManager::Default::getAssetHandle(uint64_t lowerID) {
		if (!lowerID) return AssetHandle::invalid();
		AssetHandle staticHandle{};
		uint64_t* data = static_cast<uint64_t*>(staticHandle.data());
		data[1] = 0xDEF0000000000000;
		data[0] = 0x0000000000000000 | lowerID;
		return staticHandle;
	}

	void EditorAssetManager::Default::createDefaultAssets(EditorAssetManager* assetManager) {
		if (!assetManager) return;

		const uint32_t color_white_rgba8 = 0xFFFFFFFF;
		Ref<Texture2D> texture2d_white = Texture2D::create(Image(1, 1, Image::ImageFormat::RGBA8, &color_white_rgba8));
		texture2d_white->m_AssetHandle = EditorAssetManager::Default::Texture2D_White;
		
		assetManager->m_DefaultAssets[EditorAssetManager::Default::Texture2D_White] = texture2d_white;

		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_SpriteRenderComponent] = Shader::create("../C78Editor/assets/shaders/Renderer3D_SpriteRenderComponent.glsl");
		
		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_TextCompoent] = Shader::create("../C78Editor/assets/shaders/Renderer3D_TextComponent.glsl");
		
		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_ModelCompoent] = Shader::create("../C78Editor/assets/shaders/Renderer3D_ModelComponent.glsl");

	}

	/*
	* RuntimeAssetManager
	*/

	//TODO

}