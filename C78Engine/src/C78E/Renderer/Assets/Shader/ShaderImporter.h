#pragma once

#include <C78E/Renderer/API/Shader.h>

namespace C78E {

	class ShaderImporter {
	public:
		/*
		* Imports the Shader specified in meta.fileSource
		* returns a reference to the Shader
		*/
		static Ref<Asset::Group> importShader(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle);
	};
	
}