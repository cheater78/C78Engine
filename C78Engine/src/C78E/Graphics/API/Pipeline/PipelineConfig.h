#pragma once

#include <C78E/Core/Image/MultiSample.h>
#include "PipelineType.h"

namespace C78E {

	enum class PrimitiveTopology {
		Points,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan,
		LinesAdjacency,
		LineStripAdjacency,
		TrianglesAdjacency,
		TriangleStripAdjacency,
		Patches
	};

	enum class DepthFunction {
		Never = 0,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		ALWAYS
	};

	struct PipelineConfig {
	public:
		virtual ~PipelineConfig() = default;

		virtual PipelineType getType() const = 0;
	};

	struct GraphicsPipelineConfig : public PipelineConfig {
	public:
		GraphicsPipelineConfig() = default;
		virtual ~GraphicsPipelineConfig() = default;

		virtual PipelineType getType() const override final { return PipelineType::Graphics; }
	public:
		PrimitiveTopology primitiveTopology = PrimitiveTopology::Triangles;
		bool primitiveStripRestart = false; // for strip topologies only

		bool testDepthBuffer = false;
		bool writeDepthBuffer = false;
		DepthFunction depthFunction = DepthFunction::Less;
		MultiSample multisample = MultiSample::None;

		bool backfaceCulling = false;
		bool frontFaceCulling = false;

		float lineWidth = 1.f; //TODO: requires some vk bs
		uint32_t patchControlPoints = 3; // for tessellation only
	};

	struct ComputePipelineConfig : public PipelineConfig {
	public:
		virtual ~ComputePipelineConfig() = default;

		virtual PipelineType getType() const override final { return PipelineType::Compute; }
	};

	struct RayTracingPipelineConfig : public PipelineConfig {
	public:
		virtual ~RayTracingPipelineConfig() = default;

		virtual PipelineType getType() const override final { return PipelineType::RayTracing; }
	};
}
