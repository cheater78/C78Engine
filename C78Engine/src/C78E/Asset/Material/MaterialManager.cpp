#include <C78EPCH.h>
#include "MaterialManager.h"

#include <C78E/Utils/Wavefront/WavefrontImporter.h>

namespace C78E {

	void MaterialImporter::MaterialMeta::serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {
		if (WavefrontMaterialImporter::compatible(assetDescriptor.second->fileSource))
			WavefrontMaterialImporter::MaterialMeta::serialize(assetDescriptor, appendTo);
		return;
	}

	void MaterialImporter::MaterialMeta::deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo) {
		if (WavefrontMaterialImporter::compatible(appendTo->second->fileSource))
			WavefrontMaterialImporter::MaterialMeta::deserialize(serializedMeta, appendTo);
		return;
	}

	MaterialImporter::MaterialImporter(const FilePath& assetDirectory)
		: EditorAssetManager::Importer(assetDirectory) {
	}

	MaterialImporter::~MaterialImporter() {
	}

	bool MaterialImporter::compatible(const FilePath& assetFile) {
		return WavefrontMaterialImporter::compatible(assetFile);
	}

	Ref<EditorAssetManager::ImportedAssetGroup> MaterialImporter::import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
		C78E_CORE_VALIDATE(importDescriptorMap, return nullptr, "MaterialImporter::import: importDescriptorMap was null!");
		FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
		C78E_CORE_VALIDATE(!sourceFile.empty(), return nullptr, "MeshImporter::import: verifyMapSourceFile failed!");

		if (WavefrontMaterialImporter::compatible(sourceFile))
			return dynamic_cast<WavefrontMaterialImporter*>(this)->import(importDescriptorMap);

		return nullptr;
	}

}