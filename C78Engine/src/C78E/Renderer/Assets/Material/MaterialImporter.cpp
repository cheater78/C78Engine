#include "C78EPCH.h"
#include "MaterialImporter.h"

#include <C78E/Project/Project.h>

#include <C78E/Utils/Wavefront/WavefrontSerializer.h>

namespace C78E {

	Ref<Asset::Group> MaterialLoader::importMaterial(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle) {
		FilePath ext = meta->fileSource.extension();

		if (ext == C78E_FILE_EXT_WAFEFRONT_MATERIAL) return WavefrontSerializer::importWavefront(assetDirectory / meta->fileSource);
		
		return Ref<Asset::Group>();
	}

}