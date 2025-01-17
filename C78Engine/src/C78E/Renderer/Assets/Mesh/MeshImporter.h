#pragma once
#include "Mesh.h"

namespace C78E {

	class MeshImporter {
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Asset::Group> importMesh(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle);
	};

}