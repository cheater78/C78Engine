#include "textureRenderSystem.h"
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <iostream>

namespace C78e {

	struct SimplePushConstantData {
		glm::mat4 modelMat{ 1.f };
		glm::mat4 normalMat{ 1.f };
	};

	TextureRenderSystem::TextureRenderSystem(Device& vulkDevice, VkRenderPass renderPass, ShaderSet shaders, vector<VkDescriptorSetLayout>& descriptorSetLayouts)
		: vulkDevice{ vulkDevice }, shaders{ shaders } {
		createPipelineLayout(descriptorSetLayouts);
		createPipeline(renderPass);
	}

	TextureRenderSystem::~TextureRenderSystem() {
		vkDestroyPipelineLayout(vulkDevice.device(), pipelineLayout, nullptr);
	}

	void TextureRenderSystem::createPipelineLayout(vector<VkDescriptorSetLayout> descriptorSetLayouts) {

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
			throw runtime_error("failed to create pipline layoout");
		}
	}

	void TextureRenderSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != VK_NULL_HANDLE && "Pipeline creation is dependend on a PipelineLayout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;

		vulkPipeline = make_unique<Pipeline>(
			vulkDevice,
			shaders.vertSh,
			shaders.fragSh,
			pipelineConfig
		);
	}
	void TextureRenderSystem::onUpdate(RenderTask& renderTask, vector<VkDescriptorSet> descriptorSets) {

	}

	void TextureRenderSystem::onRender(RenderTask& renderTask, vector<VkDescriptorSet>& descriptorSets, unique_ptr<DescriptorWriter>& textureDescWriter, unordered_map<unsigned int, unique_ptr<Texture>>& textures) {



		textureDescWriter->updateImage(0, &textures[0]->textureImageInfo);
		vulkPipeline->bind(renderTask.commandBuffer);

		for (auto& entry : renderTask.objects) {

			auto& obj = entry.second;

			if (obj.model == nullptr) continue;
			if (obj.textureID == -1) continue;

			


			for (int i = 0; i < descriptorSets.size(); i++) {
				vkCmdBindDescriptorSets(
					renderTask.commandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					pipelineLayout,
					i, 1,
					&descriptorSets[i],
					0,
					nullptr
				);
			}
			

			SimplePushConstantData push{};
			push.modelMat = obj.transform.mat4();
			push.normalMat = obj.transform.normalMat();

			vkCmdPushConstants(renderTask.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			obj.model->bind(renderTask.commandBuffer);
			obj.model->draw(renderTask.commandBuffer);
		}

	}

}