#pragma once
#include <C78E/Asset/Mesh/MeshManager.h>
#include <C78E/Asset/Material/MaterialManager.h>
#include <C78E/Asset/Texture/TextureManager.h>

namespace C78E {

	class GLTFImporter : public EditorAssetManager::Importer {
	public:
		struct GLTFMeta : public EditorAssetManager::Meta {
			GLTFMeta() = default;
			GLTFMeta(const GLTFMeta& other) = default;
			GLTFMeta(GLTFMeta&& other) noexcept = default;
			GLTFMeta& operator=(const GLTFMeta& other) = default;
			GLTFMeta& operator=(GLTFMeta&& other) noexcept = default;
			virtual ~GLTFMeta() override = default;

			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
		};
	public:
		GLTFImporter(const FilePath& assetDirectory);
		virtual ~GLTFImporter() override;

		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		static bool compatible(const FilePath& assetFile);
	};

}