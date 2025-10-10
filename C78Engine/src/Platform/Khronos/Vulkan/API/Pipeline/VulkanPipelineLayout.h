#pragma once
#include <C78E/Graphics/API/Pipeline/PipelineLayout.h>
#include <Platform/Khronos/Vulkan/Core/VulkanGraphicsContextItem.h>
#include <Platform/Khronos/Vulkan/API/Program/VulkanShader.h>
#include <Platform/Khronos/Vulkan/Core/Descriptor/VulkanDescriptor.h>

namespace C78E {

	VkFormat toVkFormat(ShaderDataType type);

	class VulkanPipelineLayout : public virtual VulkanGraphicsContextItem, public virtual PipelineLayout {
	public:
		VulkanPipelineLayout();
		virtual ~VulkanPipelineLayout();

		virtual PipelineType getType() const override = 0;

		const VkPipelineLayout& getVkPipelineLayout() const { return m_VkPipelineLayout; }
		const VkPipelineLayout* getVkPipelineLayoutPtr() const { return &m_VkPipelineLayout; }
		
		virtual std::vector<VkPipelineShaderStageCreateInfo> getVkPipelineShaderStageCreateInfos() const = 0;

	protected:
		VkPipelineLayout m_VkPipelineLayout;
	};

	class VulkanGraphicsPipelineLayout : public GraphicsPipelineLayout, public VulkanPipelineLayout {
	public:
		VulkanGraphicsPipelineLayout(GraphicsContext& ctx);
		virtual ~VulkanGraphicsPipelineLayout();

		virtual PipelineType getType() const override {
			return PipelineType::Graphics;
		}

		virtual bool alive() override { return false; }

		//TODO: integrate in GraphicsContextItem
		bool init();
		virtual void free() override; //TODO: integrate in GraphicsContextItem

		virtual std::vector<VkPipelineShaderStageCreateInfo> getVkPipelineShaderStageCreateInfos() const override;
	public:
		VkPipelineVertexInputStateCreateInfo getVertexInputInfo();
		const std::vector<VkDescriptorSetLayout>& getDescriptorSetLayouts() {
			return m_DescriptorSetLayouts;
		}


		
	private:
		

		bool initDescriptorSetLayouts();
		void freeDescriptorSetLayouts();

		bool initPushConstantRanges();
		void freePushConstantRanges();

		void writePipelineVertexInputLayout();
	protected:
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		std::vector<VkPushConstantRange> m_PushConstantRanges;

		std::vector<VkVertexInputBindingDescription> m_BindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;

	};

	class VulkanComputePipelineLayout : public ComputePipelineLayout, public VulkanPipelineLayout {
	public:

		virtual PipelineType getType() const override {
			return PipelineType::Compute;
		}

		virtual std::vector<VkPipelineShaderStageCreateInfo> getVkPipelineShaderStageCreateInfos() const override;
		// Ref<Shader> m_Shader;
	};

	class VulkanRayTracingPipelineLayout : public RayTracingPipelineLayout, public VulkanPipelineLayout {
	public:

		virtual PipelineType getType() const override {
			return PipelineType::RayTracing;
		}

		virtual std::vector<VkPipelineShaderStageCreateInfo> getVkPipelineShaderStageCreateInfos() const override;
	};

}
