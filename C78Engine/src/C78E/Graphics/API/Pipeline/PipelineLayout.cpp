#include "C78EPCH.h"
#include "PipelineLayout.h"

#include <C78E/Graphics/API/GraphicsInstance.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipelineLayout.h>

namespace C78E {

	PipelineType GraphicsPipelineLayout::getType() const {
		return PipelineType::Graphics;
	}

	// Instance Buffer Layouts
	void GraphicsPipelineLayout::setInstanceBufferLayout(size_t index, const InstanceBufferLayout& layout) {
		if (index < m_InstanceBufferLayouts.size()) {
			m_InstanceBufferLayouts.resize(index + 1);
		}
		m_InstanceBufferLayouts[index] = layout;
	}

	// Vertex Buffer Layouts
	void GraphicsPipelineLayout::setVertexBufferLayout(size_t index, const VertexBufferLayout& layout) {
		if (index <= m_VertexBufferLayouts.size()) {
			m_VertexBufferLayouts.resize(index + 1);
		}
		m_VertexBufferLayouts[index] = layout;
	}

	// Push Constant Layouts
	void GraphicsPipelineLayout::setPushConstantLayout(ShaderStages stages, size_t index, const UniformLayout& layout) {

	}

	// Uniform Buffer Layouts
	void GraphicsPipelineLayout::setUniformBufferLayout(ShaderStages stages, size_t binding, const UniformLayout& layout) {
		// New Binding 
		BufferBinding bufferBinding{
			.stages = stages,
			.binding = static_cast<uint32_t>(binding),
			.layout = layout
		};

		// Search existing Bindings for Collisions
		std::vector<uint32_t> obsoleteBindings;
		for (uint32_t i = 0; i < m_UniformBufferBindings.size(); i++) {
			BufferBinding& b = m_UniformBufferBindings[i];
			const bool stagesOverlap = b.stages & stages;
			const bool bindingOverlap = b.binding == binding;
			const bool bindingCollision = stagesOverlap && bindingOverlap;
			// Bindings did not collide or was the same
			if (!bindingCollision || b.layout == layout) {
				continue;
			}
			const bool exactMatch = (b.stages & stages) == stages;
			// Binding alr exits
			if (exactMatch) {
				return;
			}
			const bool fullOverride = (b.stages & stages) == b.stages;
			// new Binding makes this one obsolete
			if(fullOverride) {
				obsoleteBindings.push_back(i);
				continue;
			}
			// Binding partially matched this one -> add new, but keep unchanged stages
			b.stages &= ~stages;
		}

		// Deletion pass
		for (auto it = obsoleteBindings.rbegin(); it != obsoleteBindings.rend(); it++) {
			m_UniformBufferBindings.erase(m_UniformBufferBindings.begin() + *it);
		}

		m_UniformBufferBindings.push_back(bufferBinding);
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
