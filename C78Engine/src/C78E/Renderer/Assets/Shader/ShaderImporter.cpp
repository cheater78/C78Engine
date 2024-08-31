#include "C78EPCH.h"
#include "ShaderImporter.h"

#include <C78E/Renderer/Assets/AssetManager.h>

namespace C78E {
	Ref<Shader> ShaderImporter::importShader(AssetHandle handle, const Asset::AssetMeta& meta) {
		return Shader::create(meta.fileSource);
	}
}


