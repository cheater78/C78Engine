#pragma once

#include <C78E/Assets/Texture/Texture.h>
#include <C78E/Assets/AssetManager.h>

namespace C78E {

	//TODO:
	//	0. defualt Icons for Files
	//	1. Custom Icon Loading for Images
	//	2. Caching Custom Icons
	//	3. FileWatcher
	//
	//

	class ThumbnailSystem {
	public:
		ThumbnailSystem(Ref<AssetManager> assetManager);
		ThumbnailSystem(const ThumbnailSystem& other) = delete;
		~ThumbnailSystem();

	private:


	private:
		std::unordered_map<FileSystem::EntryType, AssetHandle> m_DefaultThumbnails = {};

	};

}
