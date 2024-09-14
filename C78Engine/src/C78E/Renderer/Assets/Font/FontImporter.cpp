#include "C78ePCH.h"
#include "FontImporter.h"

#include "C78E/Utils/Font/FontLoader.h"

namespace C78E {

	Ref<Font> FontImporter::importFont(AssetHandle handle, const Asset::AssetMeta& meta) {
		return FontLoader::loadFont(meta.fileSource);
	}

}
