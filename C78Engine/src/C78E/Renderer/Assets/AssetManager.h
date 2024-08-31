#pragma once
#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {

	using AssetMap = std::map<AssetHandle, Ref<Asset>>;
	using AssetRegistryEntry = std::pair<AssetHandle, Asset::AssetMeta>;
	using AssetRegistry = std::map<AssetHandle, Asset::AssetMeta>;

	class AssetManager { // abstract AssetManager
	public:
		AssetManager() = default;
		AssetManager(const AssetManager& other) = delete;
		~AssetManager() = default;
		
		virtual Ref<Asset> getAsset(AssetHandle handle) = 0;

		template <typename T>
		Ref<T> getAssetAs(AssetHandle assetHandle) {
			static_assert(std::is_base_of<Asset, T>::value, "AssetManager::getAssetAs: T must be derived of Asset!");
			C78_CORE_ASSERT(assetHandle, "AssetManager::getAssetAs: AssetHandle is null!");
			Ref<Asset> asset = getAsset(assetHandle);
			if (T::getClassType() != asset->getType()) {
				C78_CORE_ERROR("AssetManager::getAssetAs: Requested Type does not match the requested Asset!");
				C78_CORE_ERROR("  no cast from {} to {}", Asset::assetTypeToString(asset->getType()), Asset::assetTypeToString(T::getClassType()));
				C78_CORE_ASSERT(false);
			}
			return std::static_pointer_cast<T>(asset);
		}

		virtual bool isValid(AssetHandle handle) const = 0;
		virtual bool isLoaded(AssetHandle handle) const = 0;
		virtual Asset::AssetType getType(AssetHandle handle) const = 0;
	};
	
	class EditorAssetManager : public AssetManager {
	public:
		EditorAssetManager() = default;
		EditorAssetManager(const EditorAssetManager& other) = delete;
		~EditorAssetManager() = default;

		/*
		* Retrieves loaded Assets or otherwise loads them
		* returns a reference of the Asset
		*/ //TODO: AsyncAssetLoading -> LoadingAssetHandle(AssetHandle real, AssetHandle fallback, bool isLoaded)
		virtual Ref<Asset> getAsset(AssetHandle handle) override;

		/*
		* Validates a given AssetHandle and whether there is an Asset associated with
		* returns true if an Asset exists for the given AssetHandle
		*/
		virtual bool isValid(AssetHandle handle) const override;
		/*
		* Provides wheter an Asset associated with the given AssetHandle is loaded
		* returns true if the Asset is loaded
		*/
		virtual bool isLoaded(AssetHandle handle) const override;
		/*
		* Retrieves the AssetType of the Asset associated with the given AssetHandle
		* return the AssetType
		*/
		virtual Asset::AssetType getType(AssetHandle handle) const override;

		/* *Memory Only Assets*
		* Creates an Asset of a specified Asset, does not load the Asset, does not save the Registry
		* returns the AssetHandle of the created Asset
		*/
		AssetHandle addAsset(Asset::AssetMeta meta, Ref<Asset> asset);
		/*
		* Creates an Asset of a specified Asset source file, does not load the Asset, does not save the Registry
		* returns the AssetHandle of the created Asset
		*/
		AssetHandle importAsset(const FilePath& filepath, Asset::AssetMeta meta = Asset::c_NullAssetMeta, AssetHandle handle = AssetHandle::invalid());

		/*
		* Removes an Asset by AssetHandle from the Loaded Assets and AssetRegistry
		*/
		bool removeAsset(AssetHandle handle, bool fromDisk = false);

		/*
		* Retrieves the Asset Meta of an Asset given its Assethandle
		* returns the Asset Meta
		*/
		Asset::AssetMeta& getMeta(AssetHandle handle);
		/*
		* Retrieves the Source File of an Asset given its Assethandle
		* returns the Source File Path
		*/
		const FilePath& getFile(AssetHandle handle);

		/*
		* Provides access to the AssetRegistry
		* returns a reference to the AssetRegistry
		*/
		const AssetRegistry& getAssetRegistry() const { return m_AssetRegistry; }

		/*
		* Serializes and Exports the AssetRegistry to a yml file
		*/
		void exportAssetRegistry(const FilePath& assetRegistryPath);
		/*
		* DeSerializes and Imports the AssetRegistry from a yml file
		*/
		bool importAssetRegistry(const FilePath& assetRegistryPath);
		
	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;

		friend class ModelLoader;
	};

	class RuntimeAssetManager : public AssetManager {
	public:
		RuntimeAssetManager() = default;
		RuntimeAssetManager(const RuntimeAssetManager& other) = delete;
		~RuntimeAssetManager() = default;

		//TODO

		//loadAssets -> Assetpack.file(keep full Header in mem) -> Project -> current Scene -> all

	private:
		AssetMap m_LoadedAssets;

		friend class ModelLoader;
	};

}