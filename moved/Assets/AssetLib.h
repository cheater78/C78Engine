#include <C78E/Core/Core.h>

namespace C78E {

	template <typename T>
	class Asset;

#define NO_ASSET_SOURCE "<runtime>"

	template <typename T>
	class AssetLib {
	public:

		/*
		* Container for Assets of one specific type
		* 
		* every source of an Asset is assigned at least one name,
		* these names form groups of Asset instances.
		* which can be easily copied and are updated at once if called by their name
		*/

		AssetLib() = default;
		AssetLib(const AssetLib&) = delete;
		~AssetLib() {
			for (uint32_t id = 0; id < m_AssetsSize; id++)
				free(m_AssetHandles[id]);
			free(m_AssetHandles);
		}

		bool add(std::string name, Ref<T> asset) {
			auto& existing = find(name);
			for(uint32_t id : existing)
				addHandle(new Asset<T>(id, name, NO_ASSET_SOURCE, asset, this));
			if (!existing.empty()) return false;
			addHandle(new Asset<T>(m_NextFreeID, name, NO_ASSET_SOURCE, asset, this));
			return true;
		}

		void add(std::string name, const std::string& filepath) {
			auto& existing = find(name);
			for (uint32_t id : existing)
				addHandle(new Asset<T>(id, name, filepath, nullptr, this));
			if (!existing.empty()) return;
			addHandle(new Asset<T>(m_NextFreeID, name, filepath, nullptr, this));
		}

		void load(std::string name) {
			auto& existing = find(name);
			C78_CORE_ASSERT(!existing.empty(), "Asset can't be aquired.");
			for (uint32_t id : existing) {
				Asset<T>* asset = m_AssetHandles[id];
				if (asset->getSource() == NO_ASSET_SOURCE) continue;
				asset->update(asset->getName(), createRef<T>(asset->getSource()) );
			}
		}

		void load(std::string name, const std::string& filepath) {
			addHandle(new Asset<T>(m_NextFreeID, name, filepath, createRef<T>(filepath), this));
		}

		Asset<T> get(std::string name) {
			auto& existing = find(name);
			C78_CORE_ASSERT(!existing.empty(), "Asset can't be aquired." );
			return *m_AssetHandles[existing.front()];
		}

		std::vector<uint32_t> find(std::string name) {
			std::vector<uint32_t> ids;
			for (uint32_t id = 0; id < m_AssetsSize; id++)
				if (m_AssetHandles[id]->getName() == name)
					ids.push_back(id);
			return ids;
		}

	private:
		Asset<T>** m_AssetHandles;
		uint32_t m_NextFreeID = 0;
		uint32_t m_AssetsSize = 1;

		uint32_t addHandle(Asset<T>* handle) {
			uint32_t newIndex = m_NextFreeID;
			if (m_NextFreeID + 1 >= m_AssetsSize) {
				size_t oldSize = m_AssetsSize * sizeof(Asset<T>*);
				size_t newSize = (m_NextFreeID + 1) * sizeof(Asset<T>*);

				Asset<T>** newStorage = (Asset<T>**) malloc(static_cast<size_t>(sizeof(Asset<T>*) * (m_NextFreeID + 1)));
				memcpy_s(newStorage, newSize, m_AssetHandles, oldSize);
				free(m_AssetHandles);
				m_AssetHandles = newStorage;
			}
			m_AssetHandles[m_NextFreeID] = handle;
			updateNextHandle();
			return newIndex;
		}

		void remHandle(uint32_t id) {
			if (id >= m_AssetsSize) return;
			if(!m_AssetHandles[id])
				free(m_AssetHandles[id]);
			m_AssetHandles[id] = nullptr;
			updateNextHandle();
		}

		//find the smallest unused id
		void updateNextHandle() {
			for (uint32_t id = 0; id < m_AssetsSize; id++)
				if (!m_AssetHandles[id]) {
					m_NextFreeID = id;
					return;
				}
			m_NextFreeID = m_AssetsSize;
			return;
		}

		friend class Asset<T>;
	};

	template <typename T>
	class Asset {
	public:
		Asset(const Asset<T>& other)
			: m_LibID(other.m_Library->addHandle(this)), m_Name(other.m_Name), m_Source(other.m_Source), m_Asset(other.m_Asset), m_Library(other.m_Library)
		{ }
		~Asset()
		{
			m_Library->remHandle(m_LibID);
		}

		operator Ref<T>() { return m_Asset; }

		T& get() { return *m_Asset->get(); }
		std::string getName() { return m_Name; }
		std::string getSource() { return m_Source; }

	private:
		uint32_t m_LibID;
		std::string m_Name;
		std::string m_Source;
		Ref<T> m_Asset;
		AssetLib<T>* m_Library;

		Asset() = delete;
		Asset(uint32_t id, std::string name, std::string source, Ref<T> asset, AssetLib<T>* library)
			: m_LibID(id), m_Name(name), m_Source(source), m_Asset(asset), m_Library(library)
		{ }
		

		void update(std::string name, Ref<T> asset) { m_Name = name; m_Asset = asset; }
		void update(std::string name, Ref<T> asset, std::string source) { m_Name = name; m_Asset = asset; m_Source = source; }

		friend class AssetLib<T>;
	};
}
