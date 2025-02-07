#include <C78EPCH.h>
#include "MeshManager.h"

#include <C78E/Utils/Wavefront/WavefrontImporter.h>

namespace C78E {

	void MeshImporter::MeshMeta::serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {
		if (WavefrontMeshImporter::compatible(assetDescriptor.second->fileSource))
			WavefrontMeshImporter::WavefrontMeshMeta::serialize(assetDescriptor, appendTo);
		else { }

		return;
	}

	void MeshImporter::MeshMeta::deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo) {
		if (WavefrontMeshImporter::compatible(appendTo->second->fileSource))
			WavefrontMeshImporter::WavefrontMeshMeta::deserialize(serializedMeta, appendTo);
		else { }

		return;
	}

	MeshImporter::MeshImporter(const FilePath& assetDirectory)
		: EditorAssetManager::Importer(assetDirectory) {
	}

	MeshImporter::~MeshImporter() {
	}

	bool MeshImporter::compatible(const FilePath& assetFile) {
		return WavefrontMeshImporter::compatible(assetFile);
	}

	Ref<EditorAssetManager::ImportedAssetGroup> MeshImporter::import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
		C78E_CORE_VALIDATE(importDescriptorMap, return nullptr, "MeshImporter::import: importDescriptorMap was null!");
		FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
		C78E_CORE_VALIDATE(!sourceFile.empty(), return nullptr, "MeshImporter::import: verifyMapSourceFile failed!");

		if (WavefrontMeshImporter::compatible(sourceFile))
			return WavefrontMeshImporter(m_AssetDirectory).import(importDescriptorMap);
		
		return nullptr;
	}

}