#include "C78EPCH.h"
#include "SpirvShaderCompiler.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace C78E {

	shaderc_optimization_level toShaderCOptimizationLevel(ShaderOptimizationLevel level) {
		switch (level) {
		case ShaderOptimizationLevel::None:			return shaderc_optimization_level_zero;
		case ShaderOptimizationLevel::Size:			return shaderc_optimization_level_size;
		case ShaderOptimizationLevel::Performance:	return shaderc_optimization_level_performance;
		}
		C78E_CORE_ASSERT(false, "Unknown ShaderOptimizationLevel!");
		return shaderc_optimization_level_zero;
	}

	shaderc_shader_kind toShaderCKind(ShaderStage stage) {
		switch (stage.stage()) {
		case ShaderStage::Vertex:						return shaderc_vertex_shader;
		case ShaderStage::Fragment:						return shaderc_fragment_shader;
		case ShaderStage::Geometry:						return shaderc_geometry_shader;
		case ShaderStage::Compute:						return shaderc_compute_shader;
		case ShaderStage::TessellationControl:			return shaderc_tess_control_shader;
		case ShaderStage::TessellationEvaluation:		return shaderc_tess_evaluation_shader;
			//TODO: Add more shader stages
		default:
			C78E_CORE_ASSERT(false, "Unknown ShaderStage!");
		}
		return shaderc_glsl_infer_from_source;
	}

	SpirvShaderCompiler::SpirvShaderCompiler(const FilePath& cacheDirectory)
		: ShaderCompiler(cacheDirectory) {
	}

	const ShaderCompilationResult SpirvShaderCompiler::compile(const ShaderCompileInfo& shaderCompilationInfo) {
		C78E_CORE_TRACE("SpirvShaderCompiler::compile: Compiling Shader '{}', Stage: {}", shaderCompilationInfo.shaderName, ShaderStage::shaderStageToString(shaderCompilationInfo.stage));
		ShaderCompilationResult result;

		const FilePath cacheFile = m_CacheDirectory / shaderCompilationInfo.cacheFile;
		C78E_CORE_ASSERT(!m_CacheEnabled || FileSystem::isFile(cacheFile), "SpirvShaderCompiler::compile: Cache was enabled but cacheFile was not set correctly! (was {})", cacheFile.string());
		C78E_CORE_TRACE("SpirvShaderCompiler::compile: Caching: {}", ((m_CacheEnabled) ? "Enabled (File: " + cacheFile.string() + ")" : "Disabled"));

		// Load from cache if it exists
		if (m_CacheEnabled && FileSystem::exists(cacheFile)) {
			C78E_CORE_TRACE("SpirvShaderCompiler::compile: Loading cached SPIR-V binary from {}", cacheFile.string());
			result.binary = FileSystem::readFileBinary(cacheFile);
			result.cacheFile = cacheFile;
			result.success = true;

			return result;
		}

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetOptimizationLevel(toShaderCOptimizationLevel(shaderCompilationInfo.optimizationLevel));
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version::shaderc_env_version_vulkan_1_3);
		const shaderc::SpvCompilationResult module = 
			compiler.CompileGlslToSpv(
				shaderCompilationInfo.shaderSource,
				toShaderCKind(shaderCompilationInfo.stage),
				shaderCompilationInfo.shaderName.c_str(),
				options
			);
		// compilation result
		result.success = (module.GetCompilationStatus() == shaderc_compilation_status_success);
		result.message = module.GetErrorMessage();
		result.cacheFile = (m_CacheEnabled) ? cacheFile : FilePath();
		result.binary = (result.success) ? createRef<ScopedBuffer>(std::distance(module.begin(), module.end()) * sizeof(uint32_t), module.begin()) : nullptr;
		C78E_CORE_VALIDATE(result.success, return result, "SpirvShaderCompiler::compile: Shader compilation failed! \n{}", result.message);

		// Store compiled binary in cache
		if (m_CacheEnabled) {
			const bool cacheWriteSuccess = FileSystem::writeFile(cacheFile, result.binary);
			C78E_CORE_SOFT_VALIDATE(cacheWriteSuccess, "SpirvShaderCompiler::compile: writing cache file failed!");
		}
		
		return result;
	}

}
