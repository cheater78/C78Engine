#pragma once
#include <C78E/Asset/Font/FontManager.h>

namespace C78E {

	class MSDFImporter : public FontImporter {
	public:
		struct MSDFMeta : public FontImporter::FontMeta {
			MSDFMeta() = default;
			MSDFMeta(const MSDFMeta& other) = default;
			MSDFMeta(MSDFMeta&& other) noexcept = default;
			MSDFMeta& operator=(const MSDFMeta& other) = default;
			MSDFMeta& operator=(MSDFMeta&& other) noexcept = default;
			virtual ~MSDFMeta() override = default;


			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
		};
	public:
		MSDFImporter(const FilePath& assetDirectory);
		virtual ~MSDFImporter() override;

		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		static bool compatible(const FilePath& assetFile);
	};

}