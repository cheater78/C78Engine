#include "basicRenderSystem.h"
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <vulkan/vulkan_core.h>

#include "entt/entt.hpp"
#include "Engine/engine.h"

#include <iostream>


namespace C78e {

	RenderSystem::RenderSystem(VulkanDevice& vulkDevice, VkRenderPass renderPass, ShaderSet shaders, vector<VkDescriptorSetLayout>& descriptorSetLayouts) : vulkDevice{ vulkDevice }, shaders{ shaders } {
		createPipelineLayout(descriptorSetLayouts);
		createPipeline(renderPass);
	}

	RenderSystem::~RenderSystem() {
		vkDestroyPipelineLayout(vulkDevice.device(), pipelineLayout, nullptr);
	}

	void RenderSystem::createPipelineLayout(vector<VkDescriptorSetLayout> descriptorSetLayouts) {

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

	void RenderSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != VK_NULL_HANDLE && "Pipeline creation is dependend on a PipelineLayout");

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;

		vulkPipeline = make_unique<VulkanPipeline>(
			vulkDevice,
			shaders.vertSh,
			shaders.fragSh,
			pipelineConfig
		);
	}

	void RenderSystem::onUpdate(RenderTask& renderTask, vector<VkDescriptorSet&> descriptorSets) {

	}


	void RenderSystem::onRender(RenderTask& renderTask, vector<VkDescriptorSet&> descriptorSets) {
		
		

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

		vulkPipeline->bind(renderTask.commandBuffer);

		entt::basic_view objects = renderTask.scene->reg().view<MeshComponent>();

		
		for (auto& entID : objects) {
			Entity entity{ entID, renderTask.scene };
			MeshComponent& mesh = entity.getComponent<MeshComponent>();
			TransformComponent& transform = entity.getComponent<TransformComponent>();
			TagComponent& tag = entity.getComponent<TagComponent>();

			SimplePushConstantData push{};
			push.modelMat = transform.mat4();
			push.normalMat = transform.normalMat();

			vkCmdPushConstants(renderTask.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

			for (auto& [ uuid, model ] : renderTask.models) {
				if (model.m_UUID == entity.getUUID()) {
					model.bind(renderTask.commandBuffer);
					model.draw(renderTask.commandBuffer);
					break;
				}
			}
		}
	}

}