#pragma once
#include <C78E/Assets/Asset/Asset.h>

namespace C78E {

	using AssetMap = std::map<AssetHandle, Ref<Asset>>;
	using AssetRegistry = std::map<AssetHandle, Asset::AssetMeta>;

	class AssetManagerBase { // abstract Base AssetManager
	public:
		virtual Ref<Asset> getAsset(AssetHandle handle) = 0;

		virtual bool isValid(AssetHandle handle) const = 0;
		virtual bool isLoaded(AssetHandle handle) const = 0;
		virtual Asset::AssetType getType(AssetHandle handle) const = 0;
	};



	class AssetManager { // static AssetManager "Access Point"
	public:
		template<typename T>
		static Ref<T> getAsset(AssetHandle handle) {
			Ref<Asset> asset = getCurrentProjectsAssetManager()->getAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}
		static bool isValid(AssetHandle handle);
		static bool isLoaded(AssetHandle handle);
		static Asset::AssetType getType(AssetHandle handle);
	private:
		static Ref<AssetManagerBase> getCurrentProjectsAssetManager();
	};


	// concrete AssetManagers

	class EditorAssetManager : public AssetManagerBase {
	public:
		virtual Ref<Asset> getAsset(AssetHandle handle) override;

		virtual bool isValid(AssetHandle handle) const override;
		virtual bool isLoaded(AssetHandle handle) const override;
		virtual Asset::AssetType getType(AssetHandle handle) const override;

		void loadAsset(const FilePath& filepath);

		const Asset::AssetMeta& getMeta(AssetHandle handle) const;
		const FilePath& getFile(AssetHandle handle) const;

		const AssetRegistry& getAssetRegistry() const { return m_AssetRegistry; }

		void serializeAssetRegistry();
		bool deserializeAssetRegistry();
	protected:
		AssetHandle addAsset(Asset::AssetMeta meta, Ref<Asset> asset); // TODO: memory-only assets -> kinda done
	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;

		friend class ModelLoader;
	};

	class RuntimeAssetManager : public AssetManagerBase {
	public:
		//TODO

		//loadAssets -> Assetpack.file(keep full Header in mem) -> Project -> current Scene -> all

	private:
		AssetMap m_LoadedAssets;
	};

}