#include "C78EPCH.h"
#include "FontManager.h"

#include <C78E/Utils/MSDFAtlasGen/MSDFImporter.h>

namespace C78E {

	void FontImporter::FontMeta::serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {
		AssetHandle& handle = assetDescriptor.first;
		Ref<FontMeta> meta = std::static_pointer_cast<FontMeta>(assetDescriptor.second);

		appendTo << YAML::Key << "FontSize" << YAML::Value << meta->fontSize;

		if (MSDFImporter::compatible(assetDescriptor.second->fileSource))
			MSDFImporter::MSDFMeta::serialize(assetDescriptor, appendTo);
		else { }

		return;
	}

	void FontImporter::FontMeta::deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo) {
		Ref<FontMeta> meta = std::static_pointer_cast<FontMeta>(appendTo->second);

		YAML::Node fontSizeNode = serializedMeta["FontSize"];
		C78E_CORE_VALIDATE(
			fontSizeNode,
			return,
			"FontImporter::FontMeta::deserialize: Corrupted FontMeta!\n    FontSize: {}",
			(fontSizeNode) ? fontSizeNode.as<std::string>() : "<corrupted>"
		);
		meta->fontSize = fontSizeNode.as<size_t>();

		if (MSDFImporter::compatible(appendTo->second->fileSource))
			MSDFImporter::MSDFMeta::deserialize(serializedMeta, appendTo);
		else { }

		appendTo->second = meta;
		return;
	}

	FontImporter::FontImporter(const FilePath& assetDirectory)
		: EditorAssetManager::Importer(assetDirectory) {
	}

	FontImporter::~FontImporter() {
	}

	bool FontImporter::compatible(const FilePath& assetFile) {
		return MSDFImporter::compatible(assetFile);
	}

	Ref<EditorAssetManager::ImportedAssetGroup> FontImporter::import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
		C78E_CORE_VALIDATE(importDescriptorMap, return nullptr, "FontImporter::import: importDescriptorMap was null!");
		FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
		C78E_CORE_VALIDATE(!sourceFile.empty(), return nullptr, "MeshImporter::import: verifyMapSourceFile failed!");

		if (MSDFImporter::compatible(sourceFile))
			return MSDFImporter(m_AssetDirectory).import(importDescriptorMap);

		return nullptr;
	}

}