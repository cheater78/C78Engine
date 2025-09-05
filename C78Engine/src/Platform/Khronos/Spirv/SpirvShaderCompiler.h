#pragma once
#include <C78E/Graphics/API/Program/ShaderCompiler.h>

namespace C78E {

	class SpirvShaderCompiler : public ShaderCompiler {
	public:
		SpirvShaderCompiler(const FilePath& cacheDirectory = {});
		virtual ~SpirvShaderCompiler() = default;

		virtual const ShaderCompilationResult compile(const ShaderCompileInfo& haderCompilationInfo) override;
	};

}
