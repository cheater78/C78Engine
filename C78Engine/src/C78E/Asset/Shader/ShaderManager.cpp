#include "C78EPCH.h"
#include "ShaderManager.h"

namespace C78E {

	void ShaderImporter::ShaderMeta::serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {

		return;
	}

	void ShaderImporter::ShaderMeta::deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo) {

		return;
	}

	ShaderImporter::ShaderImporter(const FilePath& assetDirectory)
		: EditorAssetManager::Importer(assetDirectory) {
	}

	ShaderImporter::~ShaderImporter() {
	}

	Ref<EditorAssetManager::ImportedAssetGroup> ShaderImporter::import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
		C78E_CORE_VALIDATE(importDescriptorMap, return nullptr, "ShaderImporter::import: importDescriptorMap was null!");
		FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
		C78E_CORE_VALIDATE(!sourceFile.empty(), return nullptr, "ShaderImporter::import: verifyMapSourceFile failed!");
		Ref<EditorAssetManager::ImportedAssetGroup> importedAssets = createRef<EditorAssetManager::ImportedAssetGroup>();

		Ref<Shader> shader = Shader::create(m_AssetDirectory / sourceFile);
		shader->handle() = importDescriptorMap->begin()->first;

		EditorAssetManager::AssetLinkedMeta assetLinkedMeta{ shader, importDescriptorMap->begin()->second };
		importedAssets->insert(assetLinkedMeta);

		return importedAssets;
	}

	bool ShaderImporter::compatible(const FilePath& assetFile) {
		return assetFile.has_extension() && (
			assetFile.extension() == C78E_FILE_EXT_SHADER_GLSL ||
			assetFile.extension() == C78E_FILE_EXT_SHADER_HLSL
		);
	}

}