#include "C78EPCH.h"
#include "FontImporter.h"

#include "C78E/Utils/Font/FontLoader.h"

namespace C78E {

	Ref<Asset::Group> FontImporter::importFont(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle) {
		C78E_CORE_VALIDATE(meta->type == Asset::Type::Font, return nullptr, "FontImporter::importFont: Asset::Meta type must be Font!");
		Ref<Font::Meta> fontMeta = std::static_pointer_cast<Font::Meta>(meta);
		Ref<Asset::Group> assets = createRef<Asset::Group>();
		assets->emplace(std::static_pointer_cast<Asset>(FontLoader::loadFont(assetDirectory / meta->fileSource, fontMeta->fontSize, assetDirectory / C78E_ASSETLOCATION_CACHE / C78E_ASSETLOCATION_FONT / meta->fileSource.parent_path() / (meta->name + ".png"))), fontMeta);
		return assets;
	}

}
