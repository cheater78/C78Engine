#include "C78E/Core/Log/SmartLog.h"
#include "C78E/Utils/StdUtils.h"
#include "C78EPCH.h"
#include "ShaderCompiler.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Spirv/SpirvShaderCompiler.h>

namespace C78E {

	Ref<ShaderCompiler> ShaderCompiler::create(const FilePath& cacheDirectory) {
		switch (GraphicsInstance::api()) {
		case API::Vulkan:
			return createRef<SpirvShaderCompiler>(cacheDirectory);
		default:
			C78E_CORE_ASSERT(false, "ShaderCompiler::create: Unsupported Graphics API!");
			return nullptr;
		}
	}

	StageShaderSourceCode ShaderCompiler::spliceShaderSourceCode(const std::string_view rawSourceCode) {
		constexpr std::string_view typeToken = "#type ";
		constexpr size_t typeTokenLength = typeToken.length();

		C78E_CORE_TRACE("ShaderCompiler::spliceShaderSourceCode: Splicing Shader Source Code into Stages...");
		StageShaderSourceCode shaderSources;

		// split source code by type token, ignore first element as it is before the first type token
		std::vector<std::string_view> rawShaderStageCodeBlocks = std::split(rawSourceCode, typeToken);
		C78E_CORE_VALIDATE(rawShaderStageCodeBlocks.size() > 1, return shaderSources, "ShaderCompiler::spliceShaderSourceCode: No Shader Type Tokens found in Source Code! Given:\n{}", rawSourceCode);
		C78E_CORE_TRACE("ShaderCompiler::spliceShaderSourceCode: Found {} Raw Shader Code Blocks...", rawShaderStageCodeBlocks.size() - 1);

		for (size_t i = 1; i < rawShaderStageCodeBlocks.size(); i++) {
			const std::string_view rawShaderCodeBlock = rawShaderStageCodeBlocks[i];

			const size_t localTypeLineEnding = std::str_find_first_line_ending(rawShaderCodeBlock);
			C78E_CORE_VALIDATE(localTypeLineEnding != std::string::npos, break, "ShaderCompiler::spliceShaderSourceCode: Syntax error, NewLine missing after type specification! Given:\n{}", rawShaderCodeBlock);
			const std::string_view shaderTypeString = rawShaderCodeBlock.substr(0, localTypeLineEnding);
			const ShaderStage shaderStage = ShaderStage::parseShaderStageFromSourceString(shaderTypeString);

			// find shader code block, after the shader type declaration line(ending)
			const size_t postTypeCodeBlockOffset = std::str_find_first_not_line_ending(rawShaderCodeBlock.substr(localTypeLineEnding));
			C78E_CORE_VALIDATE(postTypeCodeBlockOffset != std::string::npos, break, "ShaderCompiler::spliceShaderSourceCode: No Shader Code found for stage({})! Given:\n{}", shaderTypeString, rawShaderCodeBlock);
			const size_t localShaderCodeBlockOffset = localTypeLineEnding + postTypeCodeBlockOffset;

			const std::string_view shaderSourceCodeBlock = std::string_view(rawShaderCodeBlock.begin() + localShaderCodeBlockOffset, rawShaderCodeBlock.end());

			C78E_CORE_TRACE("ShaderCompiler::spliceShaderSourceCode: Found Code Block: {}, as: \n{}", ShaderStage::shaderStageToString(shaderStage), rawShaderCodeBlock);
			C78E_CORE_VALIDATE(shaderSources.find(shaderStage) == shaderSources.end(), break, "ShaderCompiler::spliceShaderSourceCode: Duplicate Shader Stage({}) found!", shaderTypeString);
			shaderSources[shaderStage] = shaderSourceCodeBlock;
		}

		return shaderSources;
	}

	FilePath ShaderCompiler::constructCacheFileName(const std::string_view shaderName, ShaderStage stage) {
		const std::string_view stageExtension = ShaderStage::shaderStageExtensionFragment(stage);
		const std::string_view spvExtension = C78E_FILE_EXT_SHADER_SPIRV_BINARY;
		
		const size_t cacheFileNameLength = shaderName.length() + stageExtension.length() + spvExtension.length();
		std::string cacheFileName;
		cacheFileName.reserve(cacheFileNameLength);
		cacheFileName.append(shaderName);
		cacheFileName.append(stageExtension);
		cacheFileName.append(spvExtension);

		return FilePath(cacheFileName);
	}

	ShaderCompiler::ShaderCompiler(const FilePath& cacheDirectory)
		: m_CacheEnabled(!cacheDirectory.empty() && FileSystem::isDirectory(cacheDirectory)), m_CacheDirectory(cacheDirectory) {
	}

	ShaderFileCompilationResult ShaderCompiler::compileFromSourceFile(const FilePath& filePath) {
		C78E_CORE_TRACE("ShaderCompiler::compileFromSourceFile: Compiling Shader from Source File: {}", filePath);
		const std::string rawSourceCode = FileSystem::readFileText(filePath);
		const StageShaderSourceCode sourceCodePerStage = spliceShaderSourceCode(rawSourceCode);

		ShaderFileCompilationResult result;
		for (const auto& [stage, sourceCode] : sourceCodePerStage) {
			ShaderCompileInfo compileInfo{
				.shaderName = FileSystem::getFileName(filePath),
				.stage = stage,
				.shaderSource = sourceCode,
				.optimizationLevel = ShaderOptimizationLevel::Performance,
				.cacheFile = m_CacheEnabled ? constructCacheFileName(FileSystem::getFileName(filePath), stage) : FilePath()
			};
			result[stage] = compile(compileInfo);
		}
		return result;
	}
}
