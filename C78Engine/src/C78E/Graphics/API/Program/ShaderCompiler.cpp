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

	StageShaderSourceCode ShaderCompiler::spliceShaderSourceCode(const std::string& rawSourceCode) {
		constexpr std::string_view typeToken = "#type";
		constexpr size_t typeTokenLength = typeToken.length();

		StageShaderSourceCode shaderSources;

		size_t pos = rawSourceCode.find_first_of(typeToken); //Start of shader type declaration line
		C78E_CORE_SOFT_VALIDATE(pos != std::string::npos, "ShaderCompiler::spliceShaderSourceCode: SourceCode did not contain a ShaderType expression(#type <shadertype>)!");
		
		while (pos != std::string::npos) {
			const size_t eol = std::str_find_first_line_ending(rawSourceCode);
			C78E_CORE_ASSERT(eol != std::string::npos, "ShaderCompiler::spliceShaderSourceCode: Syntax error, NewLine missing after ");
			
			const size_t shaderTypeStringStart = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			const size_t shaderTypeStringLength = std::str_find_first_line_ending(std::string_view(rawSourceCode.begin() + shaderTypeStringStart, rawSourceCode.end()));
			C78E_CORE_ASSERT(shaderTypeStringLength != std::string::npos, "ShaderCompiler::spliceShaderSourceCode: No LE after type specifier!");
			const size_t shaderTypeStringEnd = shaderTypeStringStart + shaderTypeStringLength;
			const std::string_view shaderTypeString = std::string_view(rawSourceCode.begin() + shaderTypeStringStart, rawSourceCode.begin() + shaderTypeStringEnd);
			const ShaderStage shaderStage = ShaderStage::parseShaderStageFromSourceString(shaderTypeString);

			const size_t shaderCodeStart = std::str_find_first_not_line_ending(std::string_view(rawSourceCode.begin() + shaderTypeStringEnd, rawSourceCode.end()));
			C78E_CORE_ASSERT(shaderCodeStart != std::string::npos, "ShaderCompiler::spliceShaderSourceCode: No Shader Code found for stage({})!", shaderTypeString);
			pos = rawSourceCode.find(typeToken, eol); // find next shader type declaration line, else remaining is the last shader code block

			shaderSources[shaderStage] = (pos == std::string::npos) ? rawSourceCode.substr(shaderCodeStart) : rawSourceCode.substr(shaderCodeStart, pos - shaderCodeStart);
		}

		return shaderSources;
	}

	FilePath ShaderCompiler::constructCacheFileName(const std::string& shaderName, ShaderStage stage) {
		std::string cacheFileName = shaderName;
		cacheFileName += ShaderStage::shaderStageExtensionFragment(stage);
		cacheFileName += C78E_FILE_EXT_SHADER_SPIRV_BINARY;
		return cacheFileName;
	}

	ShaderCompiler::ShaderCompiler(const FilePath& cacheDirectory)
		: m_CacheEnabled(!cacheDirectory.empty() && FileSystem::isDirectory(cacheDirectory)), m_CacheDirectory(cacheDirectory) {
	}

	ShaderFileCompilationResult ShaderCompiler::compileFromSourceFile(const FilePath& filePath) {
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
