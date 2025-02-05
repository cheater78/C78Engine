#pragma once
#include <C78E/Asset/Asset.h>

namespace C78E {

	/**
	 * @brief abstract AssetManager, stores and provides access to assets.
	 * all assets should be loaded and managed by an AssetManager.
	 */
	class AssetManager {
	public:
		using AssetMap = std::map<AssetHandle, Ref<Asset>>;
	public:
		AssetManager() = default;
		AssetManager(const AssetManager& other) = delete;
		virtual ~AssetManager() = default;
		
		/**
		 * @brief Retrieves an asset by its handle.
		 *
		 * @param handle The handle of the asset to retrieve.
		 * @return Ref<Asset> The asset reference.
		 */
		virtual Ref<Asset> getAsset(AssetHandle handle) = 0;

		/**
		 * @brief Retrieves an asset by its handle and downcasts it to the specified type.
		 *
		 * @tparam T The type to cdowntcast the asset to, has to be derived from Asset.
		 * @param assetHandle The handle of the asset to retrieve.
		 * @return Ref<T> The asset reference casted to the specified type.
		 */
		template <typename T>
		Ref<T> getAssetAs(AssetHandle assetHandle) {
			static_assert(std::is_base_of<Asset, T>::value, "AssetManager::getAssetAs: T must be derived of Asset!");
			C78E_CORE_ASSERT(assetHandle, "AssetManager::getAssetAs: AssetHandle is null!");
			Ref<Asset> asset = getAsset(assetHandle);
			C78E_CORE_ASSERT(T::getClassType() == asset->getType(), "AssetManager::getAssetAs: Requested Type does not match the requested Asset!\n  no cast from {} to {}", Asset::Type::assetTypeToString(asset->getType()), Asset::Type::assetTypeToString(T::getClassType()));
			return std::static_pointer_cast<T>(asset);
		}

		/**
		 * @brief Checks if an asset handle is safe to use.
		 *
		 * @param handle The handle to check.
		 * @return true If the handle is valid.
		 * @return false If the handle is not valid.
		 */
		virtual bool isValid(AssetHandle handle) const = 0;

		/**
		 * @brief Checks if an asset is loaded.
		 *
		 * @param handle The handle of the asset to check.
		 * @return true If the asset is loaded.
		 * @return false If the asset is not loaded.
		 */
		virtual bool isLoaded(AssetHandle handle) const = 0;

		/**
		 * @brief Retrieves the type of an asset by its handle.
		 *
		 * @param handle The handle of the asset.
		 * @return Asset::Type The type of the asset.
		 */
		virtual Asset::Type getType(AssetHandle handle) const = 0;
	protected:
		AssetMap m_DefaultAssets;
	};

	/**
	 * @brief RuntimeAssetManager
	 */
	class RuntimeAssetManager : public AssetManager {
	public:
		RuntimeAssetManager() = default;
		RuntimeAssetManager(const RuntimeAssetManager& other) = delete;
		virtual ~RuntimeAssetManager() = default;

		//TODO: loadAssets -> Assetpack.file(keep full Header in mem) -> Project -> current Scene -> all
	private:
		AssetMap m_LoadedAssets;
	};
	
	/**
	 * @brief EditorAssetManager
	 */
	class EditorAssetManager : public AssetManager { // Editor only
	public:
		struct Meta;
		using AssetDescriptor = std::pair<AssetHandle, Ref<Meta>>;
		using AssetLinkedMeta = std::pair<Ref<Asset>, Ref<Meta>>;

		using AssetDescriptorMap = std::map<AssetDescriptor::first_type, AssetDescriptor::second_type>;
		using AssetLinkedMetaMap = std::map<AssetLinkedMeta::first_type, AssetLinkedMeta::second_type>;

		/**
		 * @brief 
		 */
		struct Meta {
			Meta() = default;
			Meta(const Meta& other) = default;
			Meta(Meta&& other) noexcept = default;
			Meta& operator=(const Meta& other) = default;
			Meta& operator=(Meta&& other) noexcept = default;
			virtual ~Meta() = default;

			Asset::Type type = Asset::Type::None;
			std::string name = C78E_DEFAULT_ASSET_NAME;
			FilePath fileSource = ""; // relative to an AssetDirectory

			operator bool() const { return type; }

			static void serialize(AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<AssetDescriptor>& appendTo);
		};

		/**
		 * @brief 
		 */
		struct ImportedAssetGroup : public AssetLinkedMetaMap {
			ImportedAssetGroup();
			ImportedAssetGroup(std::initializer_list<AssetLinkedMeta> initList);
			~ImportedAssetGroup();

			void merge(const ImportedAssetGroup& other);
		};

		/**
		 * @brief 
		 */
		class Importer {
		public:
			Importer(const FilePath& assetDirectory);
			virtual ~Importer();

			/**
			 * @brief Imports assets given a descriptor map, which should contain all assets of the same source file,
			 * which are therefor dependent on each other.
			 * 
			 * @param importDescriptorMap The descriptor map of assets to import.
			 * @return Ref<ImportedAssetGroup> The imported asset group.
			 */
			virtual Ref<ImportedAssetGroup> import(Ref<AssetDescriptorMap> importDescriptorMap); //needs to be called with all assets of the same sourcefile

			/**
			 * @brief Checks if a file is compatible with this Importer.
			 * Each Importer Specialization should have a compatible function.
			 *
			 * @param assetFile The file to check.
			 * @return true If the file is compatible.
			 * @return false If the file is not compatible.
			 */
			static bool compatible(const FilePath& assetFile);
		protected:
			/**
			 * @brief Verifies that all AssetDescriptors in the map have the same source file.
			 *
			 * @param importDescriptorMap The descriptor map to verify.
			 * @return FilePath The verified relative source file path.
			 */
			static FilePath verifyMapSourceFile(Ref<AssetDescriptorMap> importDescriptorMap);
		protected:
			const FilePath m_AssetDirectory;
		};

		using AssetRegistryEntry = AssetDescriptor;
		using AssetRegistry = AssetDescriptorMap;
	public:
		/**
		 * @brief Constructs an EditorAssetManager with a specified asset directory.
		 *
		 * @param assetDirectory The asset directory.
		 */
		EditorAssetManager(const FilePath& assetDirectory);
		EditorAssetManager(const EditorAssetManager& other) = delete;
		virtual ~EditorAssetManager() = default;

		/**
		 * @brief Retrieves loaded assets or otherwise loads them, given their handle.
		 *
		 * @param handle The handle of the asset to retrieve.
		 * @return Ref<Asset> The asset reference.
		 */
		virtual Ref<Asset> getAsset(AssetHandle handle) override;

		/**
		 * @brief Validates a given asset handle and whether there is an asset associated with it.
		 *
		 * @param handle The handle to validate.
		 * @return true If an asset exists for the given handle.
		 * @return false If no asset exists for the given handle.
		 */
		virtual bool isValid(AssetHandle handle) const override;

		/**
		 * @brief Checks if an asset associated with the given handle is loaded.
		 *
		 * @param handle The handle of the asset to check.
		 * @return true If the asset is loaded.
		 * @return false If the asset is not loaded.
		 */
		virtual bool isLoaded(AssetHandle handle) const override;

		/**
		 * @brief Retrieves the type of the asset associated with the given handle.
		 *
		 * @param handle The handle of the asset.
		 * @return Asset::Type The type of the asset.
		 */
		virtual Asset::Type getType(AssetHandle handle) const override;

		/**
		 * @brief Creates an asset of a specified type from Memory, does not load the asset,
		 * does not save the registry.
		 *
		 * @param meta The meta information of the asset.
		 * @param asset The asset reference.
		 * @return AssetHandle The handle of the created asset.
		 */
		AssetHandle addAsset(Ref<Meta> meta, Ref<Asset> asset); //TODO: check sourceFile handling, saving...

		/**
		 * @brief Creates an asset given an AssetDescriptor, does not load the asset,
		 * does not save the registry.
		 *
		 * @param importDescriptor The descriptor of the asset to import.
		 * @return AssetHandle The handle of the imported asset.
		 */
		AssetHandle importAsset(AssetDescriptor importDescriptor);

		/**
		 * @brief Creates an asset of a specified source file, does not load the asset, does not save the registry.
		 *
		 * @param sourceFile The source file of the asset to import.
		 * @return AssetHandle The handle of the imported asset.
		 */
		AssetHandle importAsset(const FilePath& sourceFile);

		/**
		 * @brief Removes an asset by handle from the loaded assets and asset registry.
		 *
		 * @param handle The handle of the asset to remove.
		 * @param fromDisk Whether to remove the asset from disk.
		 * @return true If the asset was removed.
		 * @return false If the asset was not removed.
		 */
		bool removeAsset(AssetHandle handle, bool fromDisk = false);

		/**
		 * @brief Reloads an asset by handle.
		 *
		 * @param handle The handle of the asset to reload.
		 * @return true If the asset was reloaded.
		 * @return false If the asset was not reloaded.
		 */
		bool reloadAsset(AssetHandle handle);

		/**
		 * @brief Retrieves the meta information of an asset given its handle.
		 *
		 * @param handle The handle of the asset.
		 * @return Ref<Meta> The meta information of the asset.
		 */
		Ref<Meta> getMeta(AssetHandle handle);

		/**
		 * @brief Retrieves the meta information of an asset given its handle, downcasted to the specified meta type.
		 *
		 * @tparam T The type to downcast the meta information to, has to be derived from EditorAssetManager::Meta.
		 * @param handle The handle of the asset.
		 * @return Ref<T> The meta information downcasted to the specified type.
		 */
		template<typename T>
		Ref<T> getMetaAs(AssetHandle handle) {
			Ref<EditorAssetManager::Meta> meta = getMeta(handle);
			static_assert(std::is_base_of<EditorAssetManager::Meta, T>::value, "AssetManager::getMetaAs: T must be derived of EditorAssetManager::Meta!");
			return std::static_pointer_cast<T>(meta);
		}

		/**
		 * @brief Retrieves the absolute source file of an asset given its handle.
		 *
		 * @param handle The handle of the asset.
		 * @return FilePath The source file path.
		 */
		FilePath getFile(AssetHandle handle);

		/**
		 * @brief Provides access to the asset registry.
		 *
		 * @return const AssetRegistry& A reference to the asset registry.
		 */
		const AssetRegistry& getAssetRegistry() const { return m_AssetRegistry; }

		/**
		 * @brief Serializes and exports the asset registry to a AssetRegistry(YAML) file.
		 *
		 * @param assetRegistryPath The path to export the asset registry to.
		 * @return true If the asset registry was exported.
		 * @return false If the asset registry was not exported.
		 */
		bool exportAssetRegistry(const FilePath& assetRegistryPath);

		/**
		 * @brief Deserializes and imports the asset registry from a AssetRegistry(YAML) file.
		 *
		 * @param assetRegistryPath The path to import the asset registry from.
		 * @return true If the asset registry was imported.
		 * @return false If the asset registry was not imported.
		 */
		bool importAssetRegistry(const FilePath& assetRegistryPath);
		
	public:
		struct Default {
		public:
			static const AssetHandle Texture2D_White;
			static const AssetHandle Shader_SpriteRenderComponent;
			static const AssetHandle Shader_TextCompoent;
			static const AssetHandle Shader_ModelCompoent;
			static const AssetHandle Shader_SkyBoxComponent;
		private:
			/**
			 * @brief Constructs a Default AssetHandle from the lower 64bit of the UUID.
			 *
			 * @param lowerID The lower ID of the asset handle.
			 * @return AssetHandle The asset handle.
			 */
			static const AssetHandle getAssetHandle(uint64_t lowerID);

			/**
			 * @brief Creates default assets for the editor asset manager.
			 *
			 * @param assetManager The editor asset manager.
			 */
			static void createDefaultAssets(EditorAssetManager* assetManager);

			friend class EditorAssetManager;
		};
	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;
		FilePath m_AssetDirectory;
	};

}