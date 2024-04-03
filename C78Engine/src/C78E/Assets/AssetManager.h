#pragma once
#include <C78E/Assets/Asset/Asset.h>

namespace C78E {

	using AssetMap = std::map<AssetHandle, Ref<Asset>>;
	using AssetRegistry = std::map<AssetHandle, Asset::AssetMeta>;


	class AssetManagerBase {
	public:
		virtual Ref<Asset> getAsset(AssetHandle handle) = 0;

		virtual bool isValid(AssetHandle handle) const = 0;
		virtual bool isLoaded(AssetHandle handle) const = 0;
		virtual Asset::AssetType getType(AssetHandle handle) const = 0;
	};

	class AssetManager {
	public:
		template<typename T>
		static Ref<T> getAsset(AssetHandle handle);
		static bool isValid(AssetHandle handle);
		static bool isLoaded(AssetHandle handle);
		static Asset::AssetType getType(AssetHandle handle);
	};

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
		// TODO: memory-only assets
		AssetHandle addAsset(Asset::AssetMeta meta, Ref<Asset> asset);
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