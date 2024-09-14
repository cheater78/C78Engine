#include "C78EPCH.h"
#include "ShaderImporter.h"

namespace C78E {
	Ref<Shader> ShaderImporter::importShader(AssetHandle handle, const Asset::AssetMeta& meta) {
		return Shader::create(meta.fileSource);
	}
}


