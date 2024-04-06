/*****************************************************************//**
 * \file   AssetLoader.h
 * \brief  
 * 
 * \author c78
 * \date   April 2024
 *********************************************************************/
#pragma once
#include "Asset.h"

namespace C78E {

	class AssetLoader {
	public:
		static Ref<Asset> loadAsset(AssetHandle handle, const Asset::AssetMeta& meta);
	private:
		
	};

}
