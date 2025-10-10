#pragma once
#include <C78E/Graphics/API/Buffer/BufferLayout.h>
#include <C78E/Graphics/API/Program/Shader.h>

#include <C78E/Graphics/API/Command/RenderPass.h>
#include "PipelineType.h"
#include <C78E/Graphics/Core/GraphicsContextItem.h>

namespace C78E {

	class PipelineLayout : public virtual GraphicsContextItem {
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

		virtual bool alive() = 0;
		virtual void free() = 0;
	public:
		// Instance Buffer Layouts
		virtual void setInstanceBufferLayout(size_t index, const InstanceBufferLayout& layout);

		// Vertex Buffer Layouts
		virtual void setVertexBufferLayout(size_t index, const VertexBufferLayout& layout);

		// Push Constant Layouts
		virtual void setPushConstantLayout(ShaderStages stages, size_t index, const UniformLayout& layout);

		// Uniform Buffer Layouts
		virtual void setUniformBufferLayout(ShaderStages stages, size_t index, const UniformLayout& layout);

		// Shaders
		void setShader(ShaderStage stage, Ref<Shader> shader);

	protected:
		std::vector<InstanceBufferLayout> m_InstanceBufferLayouts; // Instance Buffer Layouts
		std::vector<VertexBufferLayout> m_VertexBufferLayouts; // Vertex Buffer Layouts
		
		struct BufferBinding {
			ShaderStages stages;
			uint32_t binding = 0;
			UniformLayout layout;
		};
		std::vector<BufferBinding> m_UniformBufferBindings;

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
