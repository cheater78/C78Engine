#include "C78EPCH.h"
#include "AssetManager.h"

#include "Asset/AssetImporter.h"

#include <C78E/Utils/Yaml/YamlUtils.h>

#include <C78E/Renderer/API/Texture.h>
#include <C78E/Renderer/API/Shader.h>

namespace C78E {

	/*
	* EditorAssetManager
	*/

	EditorAssetManager::EditorAssetManager(const FilePath& assetDirectory)
		: m_AssetDirectory(assetDirectory) {
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
				C78E_CORE_ERROR("EditorAssetManager::importAsset: '{}' files are not supported yet!", filepath.extension().string());
				return AssetHandle::invalid();
			}
		}

		Ref<Asset> asset = AssetImporter::importAsset(m_AssetDirectory, meta, handle);
		if (asset) {
			asset->m_AssetHandle = handle;
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = meta;
			//TODO: if autosave... then here -> EditorAssetManager do
			return asset->m_AssetHandle;
		}
		C78E_CORE_ERROR("EditorAssetManager::importAsset: '{}' failed to import!", filepath.string());
		return AssetHandle::invalid();
	}

	bool EditorAssetManager::removeAsset(AssetHandle handle, bool fromDisk) {
		C78E_CORE_VALIDATE(isValid(handle), return false, "EditorAssetManager::removeAsset: Assethandle is null!");
		C78E_CORE_VALIDATE(!fromDisk, return false, "EditorAssetManager::removeAsset: TODO - Cannot remove from Disk yet!"); //TODO: implement Asset deleter
		m_AssetRegistry.erase(handle);
		m_LoadedAssets.erase(handle);
		return true;
	}

	bool EditorAssetManager::reloadAsset(AssetHandle handle) {
		C78E_CORE_VALIDATE(handle.isValid(), return false, "EditorAssetManager::reloadAsset: handle was null!");
		
		//TEMP -> handle default Assets more generally
		auto defaultAssetIndex = m_DefaultAssets.find(handle);
		if (defaultAssetIndex != m_DefaultAssets.end()) {
			Default::createDefaultAssets(this);
			return defaultAssetIndex->second != nullptr;
		}

		C78E_CORE_VALIDATE(getAssetRegistry().find(handle) != getAssetRegistry().end(), return false, "EditorAssetManager::reloadAsset: given handle is unknown!");
		Asset::AssetMeta meta = getMeta(handle);
		C78E_CORE_VALIDATE(meta != Asset::c_NullAssetMeta, return false, "EditorAssetManager::reloadAsset: handle is associated with a nullmeta Asset!");

		if (!isLoaded(handle)) {
			return getAsset(handle) != nullptr;
		}
		m_LoadedAssets[handle] = AssetImporter::importAsset(m_AssetDirectory, meta, handle);
		return m_LoadedAssets[handle] != nullptr;
	}

	Asset::AssetMeta& EditorAssetManager::getMeta(AssetHandle handle) {
		auto it = m_AssetRegistry.find(handle);
		C78E_CORE_ASSERT(it != m_AssetRegistry.end(), "EditorAssetManager::getMeta: AssetHandle not found!");
		return it->second;
	}

	FilePath EditorAssetManager::getFile(AssetHandle handle) {
		return FileSystem::normalizePath(m_AssetDirectory / getMeta(handle).fileSource);
	}

	Ref<Asset> EditorAssetManager::getAsset(AssetHandle handle) {
		if (handle.isValid() && m_DefaultAssets.find(handle) != m_DefaultAssets.end()) return m_DefaultAssets.at(handle);

		C78E_CORE_VALIDATE(isValid(handle), return nullptr, "EditorAssetManager::getAsset: handle is invalid!");
		
		if (isLoaded(handle)) return m_LoadedAssets.at(handle);
		
		Ref<Asset> asset;
		asset = AssetImporter::importAsset(m_AssetDirectory, getMeta(handle), handle); //TODO: return Default, start loading async
		C78E_CORE_VALIDATE(asset, return nullptr, "EditorAssetManager::getAsset - loading Asset failed!");
		asset->m_AssetHandle = handle;
		m_LoadedAssets[handle] = asset;
		return asset;
	}

	bool EditorAssetManager::exportAssetRegistry(const FilePath& assetRegistryPath) {
		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "AssetRegistry" << YAML::Value;

			out << YAML::BeginSeq;
			for (const auto& [handle, metadata] : m_AssetRegistry) {
				out << YAML::BeginMap;
				out << YAML::Key << "Handle" << YAML::Value << handle;
				out << YAML::Key << "Name" << YAML::Value << metadata.name;
				out << YAML::Key << "FilePath" << YAML::Value << getFile(handle);
				out << YAML::Key << "Type" << YAML::Value << Asset::assetTypeToString(metadata.type);
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // Root
		}
		return FileSystem::writeFile(assetRegistryPath, out.c_str());
	}

	bool EditorAssetManager::importAssetRegistry(const FilePath& assetRegistryPath) {
		C78E_CORE_VALIDATE(FileSystem::exists(assetRegistryPath), return false, "EditorAssetManager::deserializeAssetRegistry: AssetRegistryPath does not exist!");
		
		YAML::Node node;
		try {
			node = YAML::LoadFile(assetRegistryPath.string());
		} catch (std::runtime_error e) {
			C78E_CORE_ERROR("EditorAssetManager::importAssetRegistry: Failed to load AssetRegistry file '{0}'\n     {1}", assetRegistryPath, e.what());
			return false;
		}

		YAML::Node nodeAssetRegistry = node["AssetRegistry"];
		C78E_CORE_VALIDATE(nodeAssetRegistry, return false, "EditorAssetManager::importAssetRegistry: File does not contain an AssetRegistry!\n    File: {}", assetRegistryPath);

		for (const YAML::Node& node : nodeAssetRegistry) {
			YAML::Node nodeHandle = node["Handle"];
			YAML::Node nodeName = node["Name"];
			YAML::Node nodeFilePath = node["FilePath"];
			YAML::Node nodeType = node["Type"];

			if(nodeHandle && nodeName && nodeFilePath && nodeType) {
				AssetHandle handle = nodeHandle.as<AssetHandle>();
				Asset::AssetMeta& meta = m_AssetRegistry[handle];
				
				meta.name = nodeName.as<std::string>();
				meta.fileSource = nodeFilePath.as<std::string>();
				meta.type = Asset::assetTypeFromString(nodeType.as<std::string>());
			} else {
				C78E_CORE_ERROR("EditorAssetManager::importAssetRegistry: Corrupted Asset!\n    Handle: {}\n    Name: {}\n    FilePath: {}\n    Type: {}\n Asset will not be loaded!", 
					(nodeHandle) ? std::to_string(nodeHandle.as<AssetHandle>()) : "<corrupted>",
					(nodeName) ? nodeName.as<std::string>() : "<corrupted>",
					(nodeFilePath) ? nodeFilePath.as<std::string>() : "<corrupted>",
					(nodeType) ? nodeType.as<std::string>() : "<corrupted>"
				);
			}
		}
		return true;
	}

	AssetHandle EditorAssetManager::addAsset(Asset::AssetMeta meta, Ref<Asset> asset) {
		C78E_CORE_VALIDATE(meta.type != Asset::AssetType::None, return AssetHandle::invalid(), "EditorAssetManager::addAsset: given AssetMeta.type cannot be None!");
		C78E_CORE_VALIDATE(asset, return AssetHandle::invalid(), "EditorAssetManager::addAsset: given asset is a nullptr!");

		AssetHandle handle; // generate new handle
		asset->m_AssetHandle = handle;
		m_LoadedAssets[handle] = asset;
		m_AssetRegistry[handle] = meta;
		return handle;	
	}

	const AssetHandle EditorAssetManager::Default::Texture2D_White = EditorAssetManager::Default::getAssetHandle(1);
	const AssetHandle EditorAssetManager::Default::Shader_SpriteRenderComponent = EditorAssetManager::Default::getAssetHandle(2);
	const AssetHandle EditorAssetManager::Default::Shader_TextCompoent = EditorAssetManager::Default::getAssetHandle(3);
	const AssetHandle EditorAssetManager::Default::Shader_ModelCompoent = EditorAssetManager::Default::getAssetHandle(4);
	const AssetHandle EditorAssetManager::Default::Shader_SkyBoxComponent = EditorAssetManager::Default::getAssetHandle(5);

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

		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_SpriteRenderComponent] = Shader::create(FileSystem::C78RootDirectory / "C78Editor/assets/shaders/Renderer3D_SpriteRenderComponent.glsl");
		
		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_TextCompoent] = Shader::create(FileSystem::C78RootDirectory / "C78Editor/assets/shaders/Renderer3D_TextComponent.glsl");
		
		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_ModelCompoent] = Shader::create(FileSystem::C78RootDirectory / "C78Editor/assets/shaders/Renderer3D_ModelComponent.glsl");

		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_SkyBoxComponent] = Shader::create(FileSystem::C78RootDirectory / "C78Editor/assets/shaders/Renderer3D_SkyBoxComponent.glsl");
	}

	/*
	* RuntimeAssetManager
	*/

	//TODO

}