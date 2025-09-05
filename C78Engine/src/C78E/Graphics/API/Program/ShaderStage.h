#pragma once

namespace C78E {

	class ShaderStage {
	public:
		enum Type {
			None = 0,
			// Graphics
			Vertex,
			TessellationControl,
			TessellationEvaluation,
			Geometry,
			Fragment, Pixel = Fragment,
			// Compute
			Compute,
			// Ray Tracing, TODO: check
			RayGeneration,
			Intersection,
			AnyHit,
			ClosestHit,
			Miss,
			Callable,
		};
	public:
		static ShaderStage parseShaderStageFromSourceString(std::string_view stage);

		static ShaderStage shaderStageFromString(const std::string& stage);
		static std::string shaderStageToString(ShaderStage stage);

		static std::string_view shaderStageExtensionFragment(ShaderStage stage);

		const Type& stage() const;
	public:
		ShaderStage(Type type);
		~ShaderStage() = default;

	private:
		Type m_Type;
	};

	enum class ShaderStageBits : size_t {
		None = 0,
		// Graphics
		Vertex = 1 << ShaderStage::Vertex,
		TessellationControl = 1 << ShaderStage::TessellationControl,
		TessellationEvaluation = 1 << ShaderStage::TessellationEvaluation,
		Geometry = 1 << ShaderStage::Geometry,
		Fragment = 1 << ShaderStage::Fragment, Pixel = Fragment,
		AllGraphics = Vertex | TessellationControl | TessellationEvaluation | Geometry | Fragment,
		// Compute
		Compute = 1 << ShaderStage::Compute,
		// Ray Tracing
		RayGeneration = 1 << ShaderStage::RayGeneration,
		Intersection = 1 << ShaderStage::Intersection,
		AnyHit = 1 << ShaderStage::AnyHit,
		ClosestHit = 1 << ShaderStage::ClosestHit,
		Miss = 1 << ShaderStage::Miss,
		Callable = 1 << ShaderStage::Callable
	};

}
