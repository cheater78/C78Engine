#pragma once
#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {
	
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;
	using AssetRegistryEntry = std::pair<AssetHandle, Ref<Asset::Meta>>;
	using AssetRegistry = std::map<AssetHandle, Ref<Asset::Meta>>;

	class AssetManager {
	public:
		AssetManager() = default;
		AssetManager(const AssetManager& other) = delete;
		~AssetManager() = default;
		
		virtual Ref<Asset> getAsset(AssetHandle handle) = 0;

		template <typename T>
		Ref<T> getAssetAs(AssetHandle assetHandle) {
			static_assert(std::is_base_of<Asset, T>::value, "AssetManager::getAssetAs: T must be derived of Asset!");
			C78E_CORE_ASSERT(assetHandle, "AssetManager::getAssetAs: AssetHandle is null!");
			Ref<Asset> asset = getAsset(assetHandle);
			C78E_CORE_ASSERT(T::getClassType() == asset->getType(), "AssetManager::getAssetAs: Requested Type does not match the requested Asset!\n  no cast from {} to {}", Asset::Type::assetTypeToString(asset->getType()), Asset::Type::assetTypeToString(T::getClassType()));
			return std::static_pointer_cast<T>(asset);
		}

		virtual bool isValid(AssetHandle handle) const = 0;
		virtual bool isLoaded(AssetHandle handle) const = 0;
		virtual Asset::Type getType(AssetHandle handle) const = 0;
	};
	
	class EditorAssetManager : public AssetManager {
	public:
		EditorAssetManager(const FilePath& assetDirectory);
		EditorAssetManager(const EditorAssetManager& other) = delete;
		~EditorAssetManager() = default;

		/*
		* Retrieves loaded Assets or otherwise loads them
		* returns a reference of the Asset
		*/ //TODO: AsyncAssetLoading -> LoadingAssetHandle(AssetHandle real, AssetHandle fallback, bool isLoaded) OR return default asset and load async
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
		* Retrieves the Type of the Asset associated with the given AssetHandle
		* return the Type
		*/
		virtual Asset::Type getType(AssetHandle handle) const override;

		/* *Memory Only Assets*
		* Creates an Asset of a specified Asset, does not load the Asset, does not save the Registry
		* returns the AssetHandle of the created Asset
		*/
		AssetHandle addAsset(Ref<Asset::Meta> meta, Ref<Asset> asset);
		/*
		* Creates an Asset of a specified Asset source file, does not load the Asset, does not save the Registry
		* returns the AssetHandle of the created Asset
		*/
		AssetHandle importAsset(const FilePath& filepath, Ref<Asset::Meta> meta = nullptr, AssetHandle handle = AssetHandle::invalid());

		/*
		* Removes an Asset by AssetHandle from the Loaded Assets and AssetRegistry
		*/
		bool removeAsset(AssetHandle handle, bool fromDisk = false);

		/*
		* Removes an Asset by AssetHandle from the Loaded Assets and AssetRegistry
		*/
		bool reloadAsset(AssetHandle handle);

		/*
		* Retrieves the Asset Meta of an Asset given its Assethandle
		* returns the Asset Meta
		*/
		Ref<Asset::Meta> getMeta(AssetHandle handle);

		/*
		* Retrieves the Asset Meta of an Asset given its Assethandle
		* returns the Asset Meta
		*/
		template<typename T>
		Ref<T> getMetaAs(AssetHandle handle);

		/*
		* Retrieves the Source File of an Asset given its Assethandle
		* returns the Source File Path
		*/
		FilePath getFile(AssetHandle handle);

		/*
		* Provides access to the AssetRegistry
		* returns a reference to the AssetRegistry
		*/
		const AssetRegistry& getAssetRegistry() const { return m_AssetRegistry; }

		/*
		* Serializes and Exports the AssetRegistry to a yml file
		*/
		bool exportAssetRegistry(const FilePath& assetRegistryPath);
		/*
		* Deserializes and Imports the AssetRegistry from a yml file
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
			static const AssetHandle getAssetHandle(uint64_t lowerID);
			static void createDefaultAssets(EditorAssetManager* assetManager);

			friend class EditorAssetManager;
		};
	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;
		AssetMap m_DefaultAssets;
		FilePath m_AssetDirectory;
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