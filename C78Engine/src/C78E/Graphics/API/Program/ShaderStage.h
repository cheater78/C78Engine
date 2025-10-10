#pragma once
#include <C78E/Core/Buffer/Iterator.h>

namespace C78E {

	class ShaderStage {
	public:
		using ShaderStageType = uint8_t;
	public:
		enum Type : ShaderStageType {
			// Graphics
			Vertex = 0,
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
	public:
		ShaderStage(Type type = Type::Vertex);
		~ShaderStage() = default;

		const Type& stage() const;

		bool operator==(const ShaderStage& other) const;
	private:
		Type m_Type;
	};

	class ShaderStages {
	public:
		using ShaderStageBitsType = size_t;
	public:
		enum ShaderStageBits : ShaderStageBitsType {
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
	public:
		using StageRange = BitRange<ShaderStageBitsType>;
		using StageIterator = StageRange::Iterator;
	public:
		// TODO: toString

		static ShaderStage toShaderStage(ShaderStages stages);
	public:
		ShaderStages(ShaderStageBitsType bits = None);
		~ShaderStages() = default;

		ShaderStageBitsType raw() const;

		StageRange stages() const;

		StageIterator begin() const;
		StageIterator end() const;

		ShaderStageBitsType size() const;

		bool operator==(const ShaderStages& other) const;
		bool operator&=(const ShaderStages& other);
		operator bool() const;
	private:
		ShaderStageBitsType m_Bits;
	};

	ShaderStages operator&(const ShaderStages& a, const ShaderStages& b);
	ShaderStages operator|(const ShaderStages& a, const ShaderStages& b);
	

}

namespace std {

	template<>
	struct hash<::C78E::ShaderStage> {

		size_t operator()(const ::C78E::ShaderStage& stage) const noexcept {
			return static_cast<size_t>(static_cast<uint8_t>(stage.stage()));
		}

	};

}
