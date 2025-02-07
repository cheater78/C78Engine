#pragma once
#include <C78E/Asset/Shader/Shader.h>
#include <C78E/Asset/AssetManager.h>

namespace C78E {

	class ShaderImporter : public EditorAssetManager::Importer {
	public:
		struct ShaderMeta : public EditorAssetManager::Meta {
			ShaderMeta() = default;
			ShaderMeta(const ShaderMeta& other) = default;
			ShaderMeta(ShaderMeta&& other) noexcept = default;
			ShaderMeta& operator=(const ShaderMeta& other) = default;
			ShaderMeta& operator=(ShaderMeta&& other) noexcept = default;
			virtual ~ShaderMeta() override = default;

			
			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
		};
	public:
		ShaderImporter(const FilePath& assetDirectory);
		virtual ~ShaderImporter() override;

		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		static bool compatible(const FilePath& assetFile);
	};

}