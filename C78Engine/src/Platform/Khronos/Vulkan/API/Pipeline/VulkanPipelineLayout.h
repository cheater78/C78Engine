#pragma once
#include <C78E/Graphics/API/Pipeline/PipelineLayout.h>
#include <Platform/Khronos/Vulkan/API/Program/VulkanShader.h>

namespace C78E {

	VkFormat toVkFormat(ShaderDataType type);
	
	class VulkanGraphicsContext;

	class VulkanPipelineLayout : public PipelineLayout {
	public:
		VulkanPipelineLayout(GraphicsContext& ctx);
		virtual ~VulkanPipelineLayout();

		const VkPipelineLayout& getVkPipelineLayout() const { return m_Layout; }
		const VkPipelineLayout* getVkPipelineLayoutPtr() const { return &m_Layout; }
		
		virtual std::vector<VkPipelineShaderStageCreateInfo> getVkPipelineShaderStageCreateInfos() const = 0;

	protected:
		VulkanGraphicsContext& m_GraphicsContext;
		Ref<VulkanDevice> m_Device = nullptr;
		VkPipelineLayout m_Layout;
	};

	class VulkanGraphicsPipelineLayout : public GraphicsPipelineLayout, public VulkanPipelineLayout {
	public:
		VulkanGraphicsPipelineLayout(GraphicsContext& ctx);
		virtual ~VulkanGraphicsPipelineLayout() = default;

		virtual std::vector<VkPipelineShaderStageCreateInfo> getVkPipelineShaderStageCreateInfos() const override;
	public:
		VkPipelineVertexInputStateCreateInfo getVertexInputInfo();

	private:
		void writePipelineLayoutInputLayout();
		void writePipelineVertexInputLayout();
	// protected:
		// std::vector<BufferLayout> m_InstanceBufferLayouts; // Instance Buffer Layouts
		// std::vector<BufferLayout> m_VertexBufferLayouts; // Vertex Buffer Layouts
		// std::map<ShaderStage, std::vector<BufferLayout>> m_PushConstantLayouts; // Push Constant Layouts per Stage
		// 
		// std::unordered_map<ShaderStage, Ref<Shader>> m_Shaders;
	protected:
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		std::vector<VkPushConstantRange> m_PushConstantRanges;

		std::vector<VkVertexInputBindingDescription> m_BindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;

	};

	class VulkanComputePipelineLayout : public ComputePipelineLayout, public VulkanPipelineLayout {
	public:

		virtual std::vector<VkPipelineShaderStageCreateInfo> getVkPipelineShaderStageCreateInfos() const override {
			return std::vector<VkPipelineShaderStageCreateInfo>();
		}
	// protected:
		// Ref<Shader> m_Shader;
	};

	class VulkanRayTracingPipelineLayout : public RayTracingPipelineLayout, public VulkanPipelineLayout {
	public:

		virtual std::vector<VkPipelineShaderStageCreateInfo> getVkPipelineShaderStageCreateInfos() const override {
			return std::vector<VkPipelineShaderStageCreateInfo>();
		}
	};

}
