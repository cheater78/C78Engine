#pragma once
#include <C78E/Asset/Font/Font.h>
#include <C78E/Asset/AssetManager.h>

namespace C78E {

	class FontImporter : public EditorAssetManager::Importer {
	public:
		struct FontMeta : public EditorAssetManager::Meta {
			FontMeta() = default;
			FontMeta(const FontMeta& other) = default;
			FontMeta(FontMeta&& other) noexcept = default;
			FontMeta& operator=(const FontMeta& other) = default;
			FontMeta& operator=(FontMeta&& other) noexcept = default;
			virtual ~FontMeta() = default;

			size_t fontSize = 64; // Character Pixel Size

			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
		};
	public:
		FontImporter(const FilePath& assetDirectory);
		virtual ~FontImporter();

		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		static bool compatible(const FilePath& assetFile);
	};

}