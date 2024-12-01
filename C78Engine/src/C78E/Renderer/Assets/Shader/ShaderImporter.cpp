#include "C78EPCH.h"
#include "ShaderImporter.h"

namespace C78E {
	Ref<Shader> ShaderImporter::importShader(const FilePath& assetDirectory, const Asset::AssetMeta& meta, AssetHandle handle) {
		return Shader::create(assetDirectory / meta.fileSource);
	}
}


