#include "C78EPCH.h"
#include "ShaderImporter.h"

namespace C78E {
	Ref<Asset::Group> ShaderImporter::importShader(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle) {
		Ref<Asset::Group> assets = createRef<Asset::Group>();
		assets->emplace(Shader::create(assetDirectory / meta->fileSource), meta);
		return assets;
	}
}


