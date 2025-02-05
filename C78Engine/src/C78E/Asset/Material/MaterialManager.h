#pragma once
#include <C78E/Asset/Material/Material.h>
#include <C78E/Asset/AssetManager.h>

namespace C78E {

	class MaterialImporter : public EditorAssetManager::Importer {
	public:
		struct MaterialMeta : public EditorAssetManager::Meta {
			MaterialMeta() = default;
			MaterialMeta(const MaterialMeta& other) = default;
			MaterialMeta(MaterialMeta&& other) noexcept = default;
			MaterialMeta& operator=(const MaterialMeta& other) = default;
			MaterialMeta& operator=(MaterialMeta&& other) noexcept = default;
			virtual ~MaterialMeta() = default;

			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
		};
	public:
		MaterialImporter(const FilePath& assetDirectory);
		virtual ~MaterialImporter();

		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		static bool compatible(const FilePath& assetFile);
	};

}