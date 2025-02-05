#pragma once
#include <C78E/Asset/Mesh/MeshManager.h>
#include <C78E/Asset/Material/MaterialManager.h>
#include <C78E/Asset/Texture/TextureManager.h>

namespace C78E {

	/**
	 * @brief Imports Wavefront Model (.obj) files, imports all contained Shapes to Meshes,
	 * splits Shapes by Materials, so every Mesh has only one Material,
	 * recursivly imports all Materials contained in linked(inside the .obj) Wavefront Material (.mtl) files,
	 * recursivly imports all Textures contained in linked(inside the .mtl) image files, using TextureImporter
	 */
	class WavefrontMeshImporter : public MeshImporter {
	public:
		/**
		 * @brief Specializes MeshMeta to hold Wavefront data and an optional Material Dependecy
		 */
		struct WavefrontMeshMeta : public MeshMeta {
		public:
			WavefrontMeshMeta() = default;
			WavefrontMeshMeta(const WavefrontMeshMeta& other) = default;
			WavefrontMeshMeta(WavefrontMeshMeta&& other) noexcept = default;
			WavefrontMeshMeta& operator=(const WavefrontMeshMeta& other) = default;
			WavefrontMeshMeta& operator=(WavefrontMeshMeta&& other) noexcept = default;
			virtual ~WavefrontMeshMeta() = default;
			
			int wavefrontShapeID = -2; // the Wvf Shape id this Mesh is part of(is suffiecient as Mesh Identifier, if there is only one Material for this Shape)
			int wavefrontMaterialID = -2; // the Wvf Material id belonging to the Mesh, < 0 for no Material
			std::string wavefrontGroup = ""; // Name of the Wvf Group(opt.), TODO: Asset/Entity Grouping -> could auto create a group if not empty

			/**
			 * @brief Serializes the WavefrontMeshMeta onto an existing YAML::Emitter
			 * @param assetDescriptor containing an AssetHandle and the WavefrontMeshMeta
			 * @param appendTo the YAML::Emitter to serialize onto
			 */
			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);

			/**
			 * @brief Deserializes a YAML::Node to a WavefrontMeshMeta
			 * @param serializedMeta the YAML::Node to read from
			 * @param appendTo the AssetDescriptor to add the Information to (override the Meta with a WavefrontMeshMeta)
			 */
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);

			bool operator==(const WavefrontMeshMeta& meta);
		};

	public:
		WavefrontMeshImporter(const FilePath& assetDirectory);
		virtual ~WavefrontMeshImporter();

		/**
		 * @brief Imports Meshes and linked Materials and Textures from a specified AssetDescriptorMap, containing (opt.) existing MetaData, but atleast a sourceFile
		 * @param importDescriptorMap the AssetDescriptorMap, containing existing Info
		 * @return ImportedAssetGroup, containing all imported Assets and Metas
		 */
		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;

		/**
		 * @brief Determines whether the provided assetFile can be imported using this Importer
		 * @param assetFile to check compatibility
		 * @return whether file is supported
		 */
		static bool compatible(const FilePath& assetFile);
	};

	/**
	 * @brief Imports Wavefront Material (.mtl) files, imports all contained (Wavefront-)Materials to Materials,
	 * recursivly imports all Textures contained in linked(inside the .mtl) image files, using TextureImporter
	 */
	class WavefrontMaterialImporter : public MaterialImporter {
	public:
		/**
		 * @brief  Specializes MaterialMeta to hold its Wavefront name, and the Materials Texture Dependecies
		 */
		struct WavefrontMaterialMeta : public MaterialMeta {
		public:
			WavefrontMaterialMeta() = default;
			WavefrontMaterialMeta(const WavefrontMaterialMeta& other) = default;
			WavefrontMaterialMeta(WavefrontMaterialMeta&& other) noexcept = default;
			WavefrontMaterialMeta& operator=(const WavefrontMaterialMeta& other) = default;
			WavefrontMaterialMeta& operator=(WavefrontMaterialMeta&& other) noexcept = default;
			virtual ~WavefrontMaterialMeta() = default;

			std::string wavefrontName; // Name of the Material specified in the Wvf Material file, used to id the Material inside the Wvf Material file
			std::map<AssetHandle, std::string> dependecies; //Materials existing TextureHandles, for sourcFiles

			/**
			 * @brief  Serializes the WavefrontMaterialMeta onto an existing YAML::Emitter
			 * @param assetDescriptor containing an AssetHandle and the WavefrontMeshMeta
			 * @param appendTo the YAML::Emitter to serialize onto
			 */
			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);

			/**
			 * @brief  Deserializes a YAML::Node to a WavefrontMaterialMeta
			 * @param serializedMeta the YAML::Node to read from
			 * @param appendTo the AssetDescriptor to add the Information to (override the Meta with a WavefrontMaterialMeta)
			 */
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
			
			bool operator==(const WavefrontMaterialMeta& meta);
		};

	public:
		WavefrontMaterialImporter(const FilePath& assetDirectory);
		virtual ~WavefrontMaterialImporter();

		/**
		 * @brief Imports Materials and linked Textures from a specified AssetDescriptorMap, containing (opt.) existing MetaData, but atleast a sourceFile
		 * @param importDescriptorMap the AssetDescriptorMap, containing existing Info
		 * @return ImportedAssetGroup, containing all imported Assets and Metas
		 */
		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		
		/**
		 * @brief Determines whether the provided assetFile can be imported using this Importer
		 * @param assetFile to check compatibility
		 * @return whether file is supported
		 */
		static bool compatible(const FilePath& assetFile);
	};

}