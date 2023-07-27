#include "VulkanRenderSystem.h"


namespace C78e {

	VulkanRenderSystem::VulkanRenderSystem(VulkanDevice& device) : m_VulkDevice{ device } {

	}


	VkPipelineLayout& VulkanRenderSystem::createPipelineLayout(vector<VkDescriptorSetLayout> descriptorSetLayouts) {

		VkPipelineLayout pipelineLayout{};

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo;
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		pipelineLayoutInfo.pNext = NULL;
		pipelineLayoutInfo.flags = 0;

		if (vkCreatePipelineLayout(vulkDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw runtime_error("failed to create pipline layout");
		}

		return pipelineLayout;
	}


	Pipeline& VulkanRenderSystem::createPipeline(VkRenderPass renderPass) {

	}


}