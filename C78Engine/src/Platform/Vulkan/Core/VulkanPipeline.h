#pragma once
#include "VulkanDevice.h"


namespace C78E {

	struct PipelineConfigInfo {
		
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
		
		std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkRenderPass renderPass = VK_NULL_HANDLE;
		uint32_t subpass = 0;
	};

	class VulkanPipeline {

	public:
		VulkanPipeline(VulkanDevice &device, const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo& configInfo);
		~VulkanPipeline();

		VulkanPipeline(const  VulkanPipeline&) = delete;
		VulkanPipeline& operator=(const  VulkanPipeline&) = delete;

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		void bind(VkCommandBuffer commandBuffer);

		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
		static void enableAlphaBlending(PipelineConfigInfo& configInfo);

	private:
		VulkanDevice& m_Device;
		VkPipeline m_GraphicsPipeline;
		VkShaderModule m_VertShaderModule;
		VkShaderModule m_FragShaderModule;

		static std::vector<char> readFile(const std::string& filePath);
		void createVulkanPipeline(const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo& configInfo);

	};

}