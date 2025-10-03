#include "C78EPCH.h"
#include "PipelineLayout.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipelineLayout.h>

namespace C78E {

	PipelineType GraphicsPipelineLayout::getType() const {
		return PipelineType::Graphics;
	}

	// Instance Buffer Layouts
	void GraphicsPipelineLayout::addInstanceBufferLayout(const InstanceBufferLayout& layout) {
		m_InstanceBufferLayouts.push_back(layout);
	}

	void GraphicsPipelineLayout::setInstanceBufferLayout(size_t index, const InstanceBufferLayout& layout) {
		if (index < m_InstanceBufferLayouts.size()) {
			m_InstanceBufferLayouts.resize(index + 1);
		}
		m_InstanceBufferLayouts[index] = layout;
	}

	// Vertex Buffer Layouts
	void GraphicsPipelineLayout::addVertexBufferLayout(const VertexBufferLayout& layout) {
		m_VertexBufferLayouts.push_back(layout);
	}

	void GraphicsPipelineLayout::setVertexBufferLayout(size_t index, const VertexBufferLayout& layout) {
		if (index < m_VertexBufferLayouts.size()) {
			m_VertexBufferLayouts.resize(index + 1);
		}
		m_VertexBufferLayouts[index] = layout;
	}

	// Push Constant Layouts
	void GraphicsPipelineLayout::setPushConstantLayout(ShaderStageBits stages, size_t index, const UniformLayout& layout) {

	}

	// Uniform Buffer Layouts
	void GraphicsPipelineLayout::setUniformBufferLayout(ShaderStageBits stages, size_t index, const UniformLayout& layout) {

	}

	// Shaders
	void GraphicsPipelineLayout::setShader(ShaderStage stage, Ref<Shader> shader) {
		m_Shaders[stage] = shader;
	}



	PipelineType ComputePipelineLayout::getType() const {
		return PipelineType::Compute;
	}

	void ComputePipelineLayout::setShader(Ref<Shader> shader) {
		m_Shader = shader;
	}

	PipelineType RayTracingPipelineLayout::getType() const {
		return PipelineType::RayTracing;
	}

	void RayTracingPipelineLayout::setShader(ShaderStage stage, Ref<Shader> shader) {

	}

} // namespace C78E
