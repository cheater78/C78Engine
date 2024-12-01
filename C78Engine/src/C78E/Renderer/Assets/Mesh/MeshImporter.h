#pragma once
#include "Mesh.h"

namespace C78E {

	class MeshImporter {
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Mesh> importMesh(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle);
	};

}