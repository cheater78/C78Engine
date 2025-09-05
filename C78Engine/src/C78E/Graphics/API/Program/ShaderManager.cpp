#include "C78EPCH.h"
#include "ShaderManager.h"

namespace C78E {

	ShaderManager::ShaderManager(GraphicsContext& ctx, FilePath cacheDirectory)
		: m_GraphicsContext(ctx) {
		m_Compiler = ShaderCompiler::create(cacheDirectory);
	}

	LoadedFileShaders ShaderManager::loadShaderFromSourceFile(const FilePath& filePath) {
		ShaderFileCompilationResult fileResult = m_Compiler->compileFromSourceFile(filePath);
		LoadedFileShaders loadedShaders;
		for (auto& [stage, result] : fileResult) {
			C78E_CORE_VALIDATE(result.success, continue,
				"ShaderManager::loadShaderFromSourceFile: Failed to compile shader stage {} from file {}: \n{}", ShaderStage::shaderStageToString(stage), filePath.string(), result.message);
			loadedShaders[stage] = Shader::create(m_GraphicsContext, stage, result.binary);
		}

		return loadedShaders;
	}

	Ref<Shader> ShaderManager::loadShaderFromSource(const std::string& source, ShaderStage stage, const std::string& debugName) {
		ShaderCompileInfo compileInfo{
			.shaderName = debugName,
			.stage = stage,
			.shaderSource = source,
			.optimizationLevel = ShaderOptimizationLevel::Performance
		};
		ShaderCompilationResult result = m_Compiler->compile(compileInfo);
		C78E_CORE_VALIDATE(result.success, return nullptr,
			"ShaderManager::loadShaderFromSource: Failed to compile shader stage {} from source: \n{}", ShaderStage::shaderStageToString(stage), result.message);
		
		return Shader::create(m_GraphicsContext, stage, result.binary);
	}

}
