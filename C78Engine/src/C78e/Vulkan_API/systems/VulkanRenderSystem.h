#pragma once
#include "../vulkanPipeline.h"
#include "../vulkanDevice.h"
#include "../frameInfo.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <memory>
#include <vector>
#include <string>

namespace C78e {

	struct ShaderSet {
		std::string vertSh;
		std::string fragSh;
	};

	struct SimplePushConstantData {
		glm::mat4 modelMat{ 1.f };
		glm::mat4 normalMat{ 1.f };
	};

	class VulkanRenderSystem
	{
	public:

		VulkanRenderSystem(VulkanDevice& device);

		VkPipelineLayout& createPipelineLayout(std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
		VulkanPipeline& createPipeline(VkRenderPass renderPass);

	private:
		VulkanDevice& m_VulkDevice;
	};
}