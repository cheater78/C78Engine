#pragma once
#include <C78E/Asset/Texture/Texture.h>
#include <C78E/Asset/AssetManager.h>

namespace C78E {

	class TextureImporter : public EditorAssetManager::Importer {
	public:
		struct TextureMeta : public EditorAssetManager::Meta {
			TextureMeta() = default;
			TextureMeta(const TextureMeta& other) = default;
			TextureMeta(TextureMeta&& other) noexcept = default;
			TextureMeta& operator=(const TextureMeta& other) = default;
			TextureMeta& operator=(TextureMeta&& other) noexcept = default;
			virtual ~TextureMeta() override = default;

			Texture::Type textureType;
			Ref<Texture::Specification> specification;
			Ref<Texture::Option> option;

			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
		};
	public:
		TextureImporter(const FilePath& assetDirectory);
		virtual ~TextureImporter() override;

		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		static bool compatible(const FilePath& assetFile);
	};

}