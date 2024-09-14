#pragma once

#include <C78E/Renderer/API/Shader.h>

namespace C78E {

	class ShaderImporter {
	public:
		/*
		* Imports the Shader specified in meta.fileSource
		* returns a reference to the Shader
		*/
		static Ref<Shader> importShader(AssetHandle handle, const Asset::AssetMeta& meta);

	};



}