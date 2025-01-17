#include "C78EPCH.h"
#include "MeshImporter.h"

#include <C78E/Project/Project.h>

#include <C78E/Utils/Wavefront/WavefrontSerializer.h>

namespace C78E {

	Ref<Asset::Group> MeshImporter::importMesh(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle) {
		const FilePath ext = meta->fileSource.extension();
		Ref<Asset::Group> assets = createRef<Asset::Group>();
		if (ext == ".obj")
			assets =  WavefrontSerializer::importWavefront(assetDirectory / meta->fileSource);

		C78E_ASSERT(assets->size(), "MeshImporter::importMesh: Failed to import Mesh!");
		return Ref<Asset::Group>();
	}

}