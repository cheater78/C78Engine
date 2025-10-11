#include "C78EPCH.h"
#include "AssetManager.h"

#include <C78E/Asset/Scene/SceneManager.h>
#include <C78E/Asset/Mesh/MeshManager.h>
#include <C78E/Asset/Material/MaterialManager.h>
#include <C78E/Asset/Shader/ShaderManager.h>
#include <C78E/Asset/Texture/TextureManager.h>
#include <C78E/Asset/Font/FontManager.h>

#include <C78E/Asset/Shader/Shader.h>

namespace C78E {
	
	/*
	* RuntimeAssetManager
	*/


	/*
	* EditorAssetManager
	*/
	
	//Meta
	void EditorAssetManager::Meta::serialize(AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {
		appendTo << YAML::BeginMap;
		appendTo << YAML::Key << "Handle" << YAML::Value << assetDescriptor.first;
		appendTo << YAML::Key << "Name" << YAML::Value << assetDescriptor.second->name;
		appendTo << YAML::Key << "FilePath" << YAML::Value << assetDescriptor.second->fileSource.string();
		appendTo << YAML::Key << "Type" << YAML::Value << Asset::Type::assetTypeToString(assetDescriptor.second->type);
		switch ((uint8_t)assetDescriptor.second->type) {
		case Asset::Type::None: C78E_CORE_ERROR("EditorAssetManager::Meta::serialize: Can't serialize Meta of Asset which is Type None!"); break;
		case Asset::Type::Scene: SceneImporter::SceneMeta::serialize(assetDescriptor, appendTo); break;
		case Asset::Type::Mesh: MeshImporter::MeshMeta::serialize(assetDescriptor, appendTo); break;
		case Asset::Type::Material: MaterialImporter::MaterialMeta::serialize(assetDescriptor, appendTo); break;
		case Asset::Type::Shader: ShaderImporter::ShaderMeta::serialize(assetDescriptor, appendTo); break;
		case Asset::Type::Texture: TextureImporter::TextureMeta::serialize(assetDescriptor, appendTo); break;
		case Asset::Type::Font: FontImporter::FontMeta::serialize(assetDescriptor, appendTo); break;
		default: C78E_CORE_ERROR("EditorAssetManager::Meta::serialize: Unknown Asset Type, can't serialize Meta!"); break;
		}
		appendTo << YAML::EndMap;
		return;
	}

	void EditorAssetManager::Meta::deserialize(const YAML::Node& serializedMeta, Ref<AssetDescriptor>& appendTo) {
		C78E_CORE_VALIDATE(serializedMeta, return, "EditorAssetManager::Meta::deserialize: serializedMeta was empty!");

		YAML::Node nodeHandle = serializedMeta["Handle"];
		YAML::Node nodeName = serializedMeta["Name"];
		YAML::Node nodeFilePath = serializedMeta["FilePath"];
		YAML::Node nodeType = serializedMeta["Type"];

		C78E_CORE_VALIDATE(nodeHandle && nodeName && nodeFilePath && nodeType,
			return,
			"EditorAssetManager::importAssetRegistry: Corrupted Asset!\n    Handle: {}\n    Name: {}\n    FilePath: {}\n    Type: {}\n Asset will not be loaded!",
			(nodeHandle) ? std::to_string(nodeHandle.as<AssetHandle>()) : "<corrupted>",
			(nodeName) ? nodeName.as<std::string>() : "<corrupted>",
			(nodeFilePath) ? nodeFilePath.as<std::string>() : "<corrupted>",
			(nodeType) ? nodeType.as<std::string>() : "<corrupted>"
		);

		if (!appendTo->second)
			appendTo->second = createRef<Meta>(); // create a generic Meta if none exists, it will get replaced by a specialized Meta if necessary

		appendTo->first = nodeHandle.as<AssetHandle>();
		appendTo->second->type = Asset::Type::assetTypeFromString(nodeType.as<std::string>());
		appendTo->second->name = nodeName.as<std::string>();
		appendTo->second->fileSource = nodeFilePath.as<std::string>();
		switch ((uint8_t)appendTo->second->type) {
		case Asset::Type::None: C78E_CORE_ERROR("EditorAssetManager::Meta::deserialize: Can't deserialize Meta of Asset which is Type None!"); break;
		case Asset::Type::Scene: SceneImporter::SceneMeta::deserialize(serializedMeta, appendTo); break;
		case Asset::Type::Mesh: MeshImporter::MeshMeta::deserialize(serializedMeta, appendTo); break;
		case Asset::Type::Material: MaterialImporter::MaterialMeta::deserialize(serializedMeta, appendTo); break;
		case Asset::Type::Shader: ShaderImporter::ShaderMeta::deserialize(serializedMeta, appendTo); break;
		case Asset::Type::Texture: TextureImporter::TextureMeta::deserialize(serializedMeta, appendTo); break;
		case Asset::Type::Font: FontImporter::FontMeta::deserialize(serializedMeta, appendTo); break;
		default: C78E_CORE_ERROR("EditorAssetManager::Meta::deserialize: Unknown Asset Type, can't deserialize Meta!"); break;
		}
		return;
	}

	EditorAssetManager::ImportedAssetGroup::ImportedAssetGroup()
		: AssetLinkedMetaMap() {
	}

	EditorAssetManager::ImportedAssetGroup::ImportedAssetGroup(std::initializer_list<AssetLinkedMeta> initList)
		: AssetLinkedMetaMap(initList.begin(), initList.end()) {
	}

	EditorAssetManager::ImportedAssetGroup::~ImportedAssetGroup() {
	}

	//ImportedAssetGroup
	void EditorAssetManager::ImportedAssetGroup::merge(const ImportedAssetGroup& other) {
		insert(other.begin(), other.end());
		return;
	}

	//Importer
	EditorAssetManager::Importer::Importer(const FilePath& assetDirectory)
		: m_AssetDirectory(assetDirectory) {
	}

	EditorAssetManager::Importer::~Importer() {
	}

	bool EditorAssetManager::Importer::compatible(const FilePath& assetFile) {
		return MeshImporter::compatible(assetFile) || MaterialImporter::compatible(assetFile);
	}

	//EditorAssetManager
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

	Asset::Type EditorAssetManager::getType(AssetHandle handle) const {
		if (!isValid(handle))
			return Asset::Type::None;
		return m_AssetRegistry.at(handle)->type;
	}

	AssetHandle EditorAssetManager::importAsset(AssetDescriptor importDescriptor) {
		AssetHandle& handle = importDescriptor.first;
		Ref<Meta>& meta = importDescriptor.second; // Ref& here, bc we write to it
		FilePath fileSource = meta->fileSource;

		if (!handle.isValid())
			handle = UUID();
		if (meta->type == Asset::Type::None) {
			meta->type = Asset::Type::typeFromFile(fileSource);
			C78E_CORE_VALIDATE(meta->type != Asset::Type::None, return AssetHandle::invalid(), "EditorAssetManager::importAsset: '{}' files are not supported yet!", fileSource.extension().string());
		}

		Ref<AssetDescriptorMap> assetDescriptorMap = createRef<AssetDescriptorMap>();

		for (const AssetRegistryEntry& entry : m_AssetRegistry) // collect all Assets with the same source file
			if (entry.second->fileSource == fileSource)
				(*assetDescriptorMap)[entry.first] = entry.second;

		// add the AssetDescriptor to import
		(*assetDescriptorMap)[handle] = meta;

		Ref<ImportedAssetGroup> importAssetGroup = Importer(m_AssetDirectory).import(assetDescriptorMap);

		for (auto [asset, meta] : *importAssetGroup) {
			C78E_CORE_VALIDATE(asset, continue, "EditorAssetManager::importAsset: imported Asset was null!");
			C78E_CORE_VALIDATE(meta, continue, "EditorAssetManager::importAsset: imported Assets Meta was null!");

			asset->handle() = handle;
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = meta;
			//TODO: if autosave... then here -> EditorAssetManager do
		}
		C78E_CORE_VALIDATE(handle, return AssetHandle::invalid(), "EditorAssetManager::importAsset: '{}' failed to import!", fileSource.string());
		return handle;
	}

	AssetHandle EditorAssetManager::importAsset(const FilePath& sourceFile) {
		Ref<Meta> meta = createRef<Meta>();
		meta->fileSource = sourceFile;
		AssetDescriptor assetDescriptor{ AssetHandle::invalid(), meta };
		return importAsset(assetDescriptor);
	}


	FilePath EditorAssetManager::Importer::verifyMapSourceFile(Ref<AssetDescriptorMap> importDescriptorMap) {
		C78E_CORE_VALIDATE(importDescriptorMap, return "", "EditorAssetManager::Importer::verifyMapSourceFile: importDescriptorMap was null!");
		FilePath sourceFile;
		for (const AssetDescriptor& assetDescriptor : *importDescriptorMap) {
			C78E_CORE_VALIDATE(
				sourceFile.empty() || sourceFile == assetDescriptor.second->fileSource,
				return "",
				"EditorAssetManager::Importer::verifyMapSourceFile: sourceFiles in the Map were different!"
			);
			if (sourceFile.empty())
				sourceFile = assetDescriptor.second->fileSource;
		}
		C78E_CORE_VALIDATE(!sourceFile.empty(), return "", "EditorAssetManager::Importer::verifyMapSourceFile: importDescriptorMaps sourceFiles were all empty!");
		return sourceFile;
	}

	Ref<EditorAssetManager::ImportedAssetGroup> EditorAssetManager::Importer::import(Ref<AssetDescriptorMap> importDescriptorMap) {
		FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
		if (sourceFile.empty()) return nullptr;

		if (SceneImporter::compatible(sourceFile))
			return SceneImporter(m_AssetDirectory).import(importDescriptorMap);
		if (MeshImporter::compatible(sourceFile))
			return MeshImporter(m_AssetDirectory).import(importDescriptorMap);
		if (MaterialImporter::compatible(sourceFile))
			return MaterialImporter(m_AssetDirectory).import(importDescriptorMap);
		if (ShaderImporter::compatible(sourceFile))
			return ShaderImporter(m_AssetDirectory).import(importDescriptorMap);
		if (TextureImporter::compatible(sourceFile))
			return TextureImporter(m_AssetDirectory).import(importDescriptorMap);
		if (FontImporter::compatible(sourceFile))
			return FontImporter(m_AssetDirectory).import(importDescriptorMap);

		return nullptr;
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

		if (!isLoaded(handle))
			return getAsset(handle) != nullptr;

		C78E_CORE_VALIDATE(m_AssetRegistry.find(handle) != m_AssetRegistry.end(), return false, "EditorAssetManager::reloadAsset: given handle is unknown!");
		Ref<Meta> meta = getMeta(handle);
		C78E_CORE_VALIDATE(meta, return false, "EditorAssetManager::reloadAsset: handle is associated with a nullmeta Asset!");
		return importAsset(AssetDescriptor{ handle, meta }); // no guarantee that Asset define by Handle was loaded! TODO
	}

	Ref<EditorAssetManager::Meta> EditorAssetManager::getMeta(AssetHandle handle) {
		auto it = m_AssetRegistry.find(handle);
		C78E_CORE_VALIDATE(it != m_AssetRegistry.end(), return nullptr, "EditorAssetManager::getMeta: AssetHandle not found! ({})", std::to_string(handle));
		return it->second;
	}

	FilePath EditorAssetManager::getFile(AssetHandle handle) {
		return FileSystem::normalizePath(m_AssetDirectory / getMeta(handle)->fileSource);
	}

	Ref<Asset> EditorAssetManager::getAsset(AssetHandle handle) {
		C78E_CORE_VALIDATE(isValid(handle), return nullptr, "EditorAssetManager::getAsset: handle is invalid!");
		if (m_DefaultAssets.find(handle) != m_DefaultAssets.end()) return m_DefaultAssets.at(handle);
		
		if (isLoaded(handle)) return m_LoadedAssets[handle]; //asset loaded, just return it
		
		//asset not loaded
		Ref<Meta> meta = getMeta(handle);
		C78E_CORE_VALIDATE(meta, return nullptr, "EditorAssetManager::getAsset: no meta found!");
		AssetDescriptor assetDescriptor{ handle, meta };

		importAsset(assetDescriptor); // import the Asset
		C78E_CORE_VALIDATE(isLoaded(handle), return nullptr, "EditorAssetManager::getAsset: failed to load asset!");
		C78E_CORE_VALIDATE(m_LoadedAssets[handle], return nullptr, "EditorAssetManager::getAsset: loaded Asset is null!");
		return m_LoadedAssets[handle];
	}

	bool EditorAssetManager::exportAssetRegistry(const FilePath& assetRegistryPath) {
		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "AssetRegistry" << YAML::Value << YAML::BeginMap;
			
			for (const auto& [handle, meta] : m_AssetRegistry) {
				out << YAML::Key << "AssetRegistryEntry" << YAML::Value;
				Meta::serialize(AssetDescriptor(handle, meta), out);
			}

			out << YAML::EndMap;
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
		C78E_CORE_VALIDATE(nodeAssetRegistry && nodeAssetRegistry.IsMap(), return false, "EditorAssetManager::importAssetRegistry: File does not contain an AssetRegistry!\n    File: {}", assetRegistryPath);

		m_AssetRegistry.clear();
		for (const auto& it : nodeAssetRegistry) {
			C78E_CORE_VALIDATE(it.first.as<std::string>() == "AssetRegistryEntry", continue, "EditorAssetManager::importAssetRegistry: Yaml Entry was not flagged as AssetRegistryEntry!");
			YAML::Node node = it.second;
			C78E_CORE_VALIDATE(node && node.IsMap(), continue, "EditorAssetManager::importAssetRegistry: assetRegistryEntry was invalid!");
			Ref<AssetDescriptor> assetDescriptor = createRef<AssetDescriptor>();
			Meta::deserialize(node, assetDescriptor);
			C78E_CORE_VALIDATE(assetDescriptor, continue, "EditorAssetManager::importAssetRegistry: Failed to desirialize AssetRegistryEntry!");
			m_AssetRegistry.emplace(assetDescriptor->first, assetDescriptor->second);
		}
		return !m_AssetRegistry.empty();
	}

	AssetHandle EditorAssetManager::addAsset(Ref<Meta> meta, Ref<Asset> asset) {
		C78E_CORE_VALIDATE(meta->type != Asset::Type::None, return AssetHandle::invalid(), "EditorAssetManager::addAsset: given Meta.type cannot be None!");
		C78E_CORE_VALIDATE(asset, return AssetHandle::invalid(), "EditorAssetManager::addAsset: given asset is a nullptr!");

		AssetHandle handle; // generate new handle
		asset->handle() = handle;
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
		{
			Image image_white(1, 1, Image::ImageFormat::RGBA8, &color_white_rgba8);
			Ref<Texture2D> texture2d_white = Texture2D::create(image_white);
			texture2d_white->handle() = EditorAssetManager::Default::Texture2D_White;

			assetManager->m_DefaultAssets[EditorAssetManager::Default::Texture2D_White] = texture2d_white;
		}

		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_SpriteRenderComponent] = Shader::create(FileSystem::C78RootDirectory / "C78Editor/assets/shaders/Renderer3D_SpriteRenderComponent.glsl");
		
		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_TextCompoent] = Shader::create(FileSystem::C78RootDirectory / "C78Editor/assets/shaders/Renderer3D_TextComponent.glsl");
		
		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_ModelCompoent] = Shader::create(FileSystem::C78RootDirectory / "C78Editor/assets/shaders/Renderer3D_ModelComponent.glsl");

		assetManager->m_DefaultAssets[EditorAssetManager::Default::Shader_SkyBoxComponent] = Shader::create(FileSystem::C78RootDirectory / "C78Editor/assets/shaders/Renderer3D_SkyBoxComponent.glsl");
	}

}