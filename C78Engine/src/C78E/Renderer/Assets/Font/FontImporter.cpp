#include "C78EPCH.h"
#include "FontImporter.h"

#include "C78E/Utils/Font/FontLoader.h"

namespace C78E {

	Ref<Font> FontImporter::importFont(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle) {
		return FontLoader::loadFont(assetDirectory / meta.fileSource);
	}

}
