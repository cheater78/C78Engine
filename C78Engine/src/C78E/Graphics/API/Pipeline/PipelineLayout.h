#pragma once
#include <C78E/Graphics/API/Buffer/BufferLayout.h>
#include <C78E/Graphics/API/Program/Shader.h>

#include <C78E/Graphics/API/Command/RenderPass.h>
#include "PipelineType.h"

namespace C78E {

	class PipelineLayout {
	public:
		PipelineLayout() = default;
		virtual ~PipelineLayout() = default;

		virtual PipelineType getType() const = 0;
	};


	class GraphicsPipelineLayout : public virtual PipelineLayout {
	public:
        GraphicsPipelineLayout() = default;
        virtual ~GraphicsPipelineLayout() = default;

        virtual PipelineType getType() const override;

	public:
		// Instance Buffer Layouts
		void addInstanceBufferLayout(const InstanceBufferLayout& layout);
		void setInstanceBufferLayout(size_t index, const InstanceBufferLayout& layout);

		// Vertex Buffer Layouts
		void addVertexBufferLayout(const VertexBufferLayout& layout);
		void setVertexBufferLayout(size_t index, const VertexBufferLayout& layout);

		// Push Constant Layouts
		void setPushConstantLayout(ShaderStageBits stages, size_t index, const UniformLayout& layout);

		// Uniform Buffer Layouts
		void setUniformBufferLayout(ShaderStageBits stages, size_t index, const UniformLayout& layout);

		// Shaders
		void setShader(ShaderStage stage, Ref<Shader> shader);

	protected:
		std::vector<InstanceBufferLayout> m_InstanceBufferLayouts; // Instance Buffer Layouts
		std::vector<VertexBufferLayout> m_VertexBufferLayouts; // Vertex Buffer Layouts
		std::map<ShaderStage, std::vector<UniformLayout>> m_PushConstantLayouts; // Push Constant Layouts per Stage

		std::unordered_map<ShaderStage, Ref<Shader>> m_Shaders;
	};

	class ComputePipelineLayout : public virtual PipelineLayout {
	public:
		ComputePipelineLayout() = default;
		virtual ~ComputePipelineLayout() = default;

		virtual PipelineType getType() const override;

	public:
		void setShader(Ref<Shader> shader);

	protected:
		Ref<Shader> m_Shader;

	};

	//TODO: dummy for now, not implemented yet
	class RayTracingPipelineLayout : public virtual PipelineLayout {
	public:
		RayTracingPipelineLayout() = default;
		virtual ~RayTracingPipelineLayout() = default;
		virtual PipelineType getType() const override;
	public:
		void setShader(ShaderStage stage, Ref<Shader> shader);
	protected:
		std::unordered_map<ShaderStage, Ref<Shader>> m_Shaders; // TODO: there can be multiple shaders per stage
	};

}
