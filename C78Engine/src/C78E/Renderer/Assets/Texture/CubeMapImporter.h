#pragma once

#include <C78E/Renderer/API/Texture.h>

namespace C78E {

	class CubeMapImporter {
	public:
		static Ref<CubeMap> importCubeMap(AssetHandle handle, const Asset::AssetMeta& meta);

	};

}
