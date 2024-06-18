#pragma once
#include "C78ePCH.h"
#include "Asset.h"


namespace C78E {

	class AsyncAsset {
	public:
		AsyncAsset() = default;
		~AsyncAsset() = default;


	private:
		bool m_Loaded = false;
		Ref<Asset> m_Asset = nullptr;
		Ref<Asset> m_DefaultAsset = nullptr;

		friend class AssetImporter;
	};

}
