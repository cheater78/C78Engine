#pragma once

#include "Shader.h"
#include "ShaderCompiler.h"

namespace C78E {

	using LoadedFileShaders = std::unordered_map<ShaderStage, Ref<Shader>>;

	class ShaderManager {
	public:
		ShaderManager(GraphicsContext& ctx, FilePath cacheDirectory);
		~ShaderManager() = default;

		LoadedFileShaders loadShaderFromSourceFile(const FilePath& filePath);
		Ref<Shader> loadShaderFromSource(const std::string& source, ShaderStage stage, const std::string& debugName = "Unnamed");
		
	private:
		GraphicsContext& m_GraphicsContext;
		Ref<ShaderCompiler> m_Compiler;
	};

}
