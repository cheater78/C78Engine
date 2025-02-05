#pragma once
#include <C78E/Asset/Mesh/Mesh.h>
#include <C78E/Asset/AssetManager.h>

namespace C78E {

	class MeshImporter : public EditorAssetManager::Importer {
	public:
		struct MeshMeta : public EditorAssetManager::Meta {
			MeshMeta() = default;
			MeshMeta(const MeshMeta& other) = default;
			MeshMeta(MeshMeta&& other) noexcept = default;
			MeshMeta& operator=(const MeshMeta& other) = default;
			MeshMeta& operator=(MeshMeta&& other) noexcept = default;
			virtual ~MeshMeta() = default;

			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
		};
	public:
		MeshImporter(const FilePath& assetDirectory);
		virtual ~MeshImporter();

		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		static bool compatible(const FilePath& assetFile);
	};

}