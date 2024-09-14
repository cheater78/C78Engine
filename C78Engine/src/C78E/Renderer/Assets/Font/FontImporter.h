#pragma once

#include <C78E/Renderer/Assets/Font/Font.h>

namespace C78E {

	class FontImporter {
	public:
		static Ref<Font> importFont(AssetHandle handle, const Asset::AssetMeta& meta);
	};

}