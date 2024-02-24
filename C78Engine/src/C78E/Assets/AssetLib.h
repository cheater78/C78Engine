#pragma once
#include <C78E/Core/Core.h>

#define NO_ASSET_NAME "<uninitialized>"
#define NO_ASSET_SOURCE "<runtime>"

namespace C78E {

	template <typename T>
	class Asset;

	template <typename T>
	class AssetLib {
	public:

		/*
		* Container for Assets of one specific type
		* 
		* every source of an Asset is assigned at least one name,
		* these names form groups of Asset instances.
		* which can be easily copied, assigned and are updated if the resource gets reloaded
		*/

		AssetLib() = default;
		AssetLib(const AssetLib&) = delete;
		~AssetLib() {
			// all is Stack allocated
		}

		/*
		* Takes an alr existing Resource and adds it as an Asset,
		* if name alr has assigned Assets, they get updated with the new Resource
		* returns true if a new Asset was added
		*/
		bool add(std::string name, Ref<T> asset, std::string source = NO_ASSET_SOURCE) {
			auto existing = find(name);
			for(uint32_t id : existing)
				m_AssetHandles.at(id)->update(name, asset, source);
			if (!existing.empty()) return false;
			Asset<T>* instance = new Asset<T>(m_NextFreeID, name, source, asset, this, false);
			uint32_t index = addHandle(instance);
			m_OwnedAssets.emplace(index, instance);
			return true;
		}

		/*
		* Registers a Resource, given its filepath, does not load the Resource
		* if name alr has assigned Assets, they get replaced with the unloaded Asset
		*/
		bool reg(std::string name, const std::string& filepath) {
			auto existing = find(name);
			for (uint32_t id : existing)
				m_AssetHandles.at(id)->update(name, nullptr, filepath);
			if (!existing.empty()) return false;
			Asset<T>* instance = new Asset<T>(m_NextFreeID, name, filepath, nullptr, this);
			uint32_t index = addHandle(instance);
			m_OwnedAssets.emplace(index, instance);
			return true;
		}

		/*
		* Removes an Asset from the Lib, updates remaining Handles to be empty
		*/
		bool remove(std::string name) {
			auto existing = find(name);
			if (existing.empty()) {
				C78_CORE_WARN("AssetLib: Tried to remove Asset from Lib that does not exist! -> skipping...");
				return false;
			}
			for (uint32_t id : existing) {
				Asset<T>* asset = m_AssetHandles.at(id);
				asset->update(NO_ASSET_NAME, nullptr, NO_ASSET_SOURCE);
				if (m_OwnedAssets.contains(id)) { //C++20
					delete m_OwnedAssets.at(id);
				}
				if (m_AssetHandles.contains(id)) {
					m_AssetHandles.erase(id);
				}
			}
			return true;
		}

		/*
		* Loads an alr existing Asset, regarding its filepath
		*/
		void load(std::string name) {
			auto existing = find(name);
			C78_CORE_ASSERT(!existing.empty(), "Asset can't be loaded, it's non-existent.");
			for (uint32_t id : existing) {
				Asset<T>* asset = m_AssetHandles.at(id);
				if (!asset->loadable()) continue;
				if (asset->getSource() == NO_ASSET_SOURCE) continue;
				asset->update(asset->getName(), T::create(asset->getSource()) );
			}
		}

		/*
		* TODO
		*/
		void load(std::string name, const std::string& filepath) {
			auto existing = find(name);
			if(existing.empty())
				addHandle(new Asset<T>(m_NextFreeID, name, filepath, T::create(filepath), this));
			else {
				for (uint32_t id : existing) {
					if (!m_AssetHandles.at(id)->loadable()) continue;
					m_AssetHandles.at(id)->update(name, T::create(filepath), filepath);
				}
			}
		}

		/*
		* TODO
		*/
		Asset<T> get(std::string name) {
			auto existing = find(name);
			C78_CORE_ASSERT(!existing.empty(), "Asset can't be aquired." );
			return *m_AssetHandles.at(existing.at(0));
		}

		/*
		* TODO
		*/
		std::set<std::string> getAllNames() {
			std::set<std::string> names;
			for (auto& kv : m_AssetHandles)
				names.emplace(kv.second->getName());
			return names;
		}

		/*
		* Renames an Asset group, identified by name, to another given name
		*/
		bool rename(std::string name, std::string newName) {
			auto existing = find(name);
			for (uint32_t id : existing)
				m_AssetHandles.at(id)->rename(newName);
			return existing.empty();
		}

	private:
		std::map<uint32_t, Asset<T>*> m_AssetHandles;
		std::map<uint32_t, Asset<T>*> m_OwnedAssets;
		uint32_t m_NextFreeID = 0;

		/*
		* TODO
		*/
		std::vector<uint32_t> find(std::string name) {
			std::vector<uint32_t> ids;
			for (auto& kv : m_AssetHandles)
				if (kv.second->getName() == name)
					ids.push_back(kv.first);
			return ids;
		}

		/*
		* Adds an Asset given its Pointer at the the smallest unused ID
		*/
		uint32_t addHandle(Asset<T>* handle) {
			uint32_t newIndex = m_NextFreeID;
			m_AssetHandles.emplace(newIndex, handle);
			m_NextFreeID = queryNextSmallestHandleID(newIndex);
			return newIndex;
		}

		/*
		* Removes a specific Asset by its ID
		*/
		void remHandle(uint32_t id) {
			if (id >= m_AssetHandles.size()) return;
			if (!m_AssetHandles.contains(id)) { // dodge if Asset does not exist
				C78_CORE_WARN("AssetLib: Tried to remove AssetHandle that does not exist! -> skipping...");
				return;
			}
			m_AssetHandles.erase(id);
			/* update m_NextFreeID to smallest available: if gt the removed id => id must be the smallest */
			if (m_NextFreeID > id) m_NextFreeID = id;
		}

		/*
		* Find the smallest unused ID _above_ startID, which might be implicitly given at function call
		*/
		uint32_t queryNextSmallestHandleID(uint32_t startID = 0) {
			uint32_t lastCheckedID = startID;
			for (auto& kv : m_AssetHandles) {
				if (kv.first <= startID) continue; // skip all IDs below or equal to the given startID
				if (kv.first > lastCheckedID + 1) { // check whether there is a atleast one sized gap between used IDs, if so use it
					return lastCheckedID + 1;
				}
				else continue;
			}
			return static_cast<uint32_t>(m_AssetHandles.size()); // if there are no gaps in m_AssetHandles, return size to expand by one
		}

		friend class Asset<T>;
	};

	template <typename T>
	class Asset {
	public:
		Asset() = default; //dont use to construct, just for dynamic storage types
		Asset(const Asset<T>& other)
			: m_LibID(other.m_Library->addHandle(this)), m_Name(other.m_Name), m_Source(other.m_Source), m_Asset(other.m_Asset), m_Library(other.m_Library), m_FileUnique(other.m_FileUnique)
		{ }
		~Asset() { if(valid()) m_Library->remHandle(m_LibID); }

		/* allow implicit cast to the contained resource */
		operator Ref<T>() { return m_Asset; }

		/* register the Asset on assignment, bc other might get deleted */
		Asset& operator=(const Asset& other) {
			if (this == &other) return *this;
			this->m_Name = other.m_Name;
			this->m_Source = other.m_Source;
			this->m_Asset = other.m_Asset;
			this->m_Library = other.m_Library;
			this->m_LibID = m_Library->addHandle(this);
			return *this;
		}

		Ref<T> getRef()			{ return m_Asset; }
		T& get()				{ return *(m_Asset.get()); }
		std::string getName()	{ return m_Name; }
		std::string getSource()	{ return m_Source; }

		bool valid()
		{ return !(m_Name == NO_ASSET_NAME && m_Source == NO_ASSET_SOURCE) && m_Library != nullptr;  }

	private:
		Asset(uint32_t id, std::string name, std::string source, Ref<T> asset, AssetLib<T>* library, bool loadable = true)
			: m_LibID(id), m_Name(name), m_Source(source), m_Asset(asset), m_Library(library), m_FileUnique(loadable)
		{ }

		void rename(std::string name)
		{ m_Name = name; }

		void update(std::string name, Ref<T> asset)
		{ m_Name = name; m_Asset.swap(asset); }

		void update(std::string name, Ref<T> asset, std::string source)
		{ m_Name = name; m_Asset.swap(asset); m_Source = source; }

		void invalidate()
		{ m_Library = nullptr; }

		bool loadable()
		{ return m_FileUnique; }

	private:
		uint32_t m_LibID = -1;
		std::string m_Name = NO_ASSET_NAME;
		std::string m_Source = NO_ASSET_SOURCE;
		bool m_FileUnique = true;
		Ref<T> m_Asset = nullptr;
		AssetLib<T>* m_Library = nullptr;
		
		

		friend class AssetLib<T>;
	};

}
