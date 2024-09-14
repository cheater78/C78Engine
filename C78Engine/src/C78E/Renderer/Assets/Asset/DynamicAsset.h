#pragma once
#include <C78E/Core/Core.h>
#include "Asset.h"

namespace C78E {

	template <typename A>
	class DynamicAsset {

	private:
		Ref<Asset> m_Asset;
		AssetHandle m_AssetHandle;
	};

}
