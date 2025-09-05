#pragma once

#include "ShaderStage.h"

namespace C78E {

	using StageShaderSourceCode = std::unordered_map<ShaderStage, std::string>;

	enum class ShaderOptimizationLevel {
		None = 0,
		Size = 1,
		Performance = 2,
	};

	struct ShaderCompileInfo {
		const std::string& shaderName;
		ShaderStage stage = ShaderStage::None;
		const std::string& shaderSource;
		ShaderOptimizationLevel optimizationLevel = ShaderOptimizationLevel::Performance;
		FilePath cacheFile; // opt.
	};

	struct ShaderCompilationResult {
		bool success = false;
		std::string message;
		Ref<ScopedBuffer> binary;
		FilePath cacheFile; // opt. absolute chache file path
	};

	using ShaderFileCompilationResult = std::unordered_map<ShaderStage, ShaderCompilationResult>;

	class ShaderCompiler {
	public:
		static Ref<ShaderCompiler> create(const FilePath& cacheDirectory = {});
	public:
		static StageShaderSourceCode spliceShaderSourceCode(const std::string& rawSourceCode);
		static FilePath constructCacheFileName(const std::string& shaderName, ShaderStage stage);

	public:
		ShaderCompiler(const FilePath& cacheDirectory = {});
		virtual ~ShaderCompiler() = default;

		virtual const ShaderCompilationResult compile(const ShaderCompileInfo& shaderCompilationInfo) = 0;

	public:
		ShaderFileCompilationResult compileFromSourceFile(const FilePath& filePath);
	protected:
		bool m_CacheEnabled = true;
		FilePath m_CacheDirectory;
	};

}
