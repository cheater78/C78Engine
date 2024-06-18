#pragma once
#include "Mesh.h"

namespace C78E {

	class MeshLoader {
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Mesh> importMesh(AssetHandle handle, const Asset::AssetMeta& meta);

		// Load from filepath
		static Ref<Mesh> loadMesh(const FilePath& path);
	private:

	};

}