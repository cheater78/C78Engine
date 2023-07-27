#include "pointLightSystem.h"

#include <stdexcept>
#include <array>
#include <map>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

namespace C78e {

	PointLightSystem::PointLightSystem(Device& vulkDevice, VkRenderPass renderPass, ShaderSet shaders, vector<VkDescriptorSetLayout>& descriptorSetLayouts)
		: vulkDevice{ vulkDevice }, shaders{ shaders } {
		createPipelineLayout(descriptorSetLayouts);
		createPipeline(renderPass);
	}

	PointLightSystem::~PointLightSystem() {
		vkDestroyPipelineLayout(vulkDevice.device(), pipelineLayout, nullptr);
	}

	void PointLightSystem::createPipelineLayout(vector<VkDescriptorSetLayout> descriptorSetLayouts) {

		VkPipelineLayoutCreateInfo pipelineLayoutInfo;
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		pipelineLayoutInfo.pNext = NULL;
		pipelineLayoutInfo.flags = 0;

		if (vkCreatePipelineLayout(vulkDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw runtime_error("failed to create pipline layoout");
		}
	}

	void PointLightSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != VK_NULL_HANDLE && "Pipeline creation is dependend on a PipelineLayout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPipelineConfigInfo(pipelineConfig);
		Pipeline::enableAlphaBlending(pipelineConfig);
		pipelineConfig.attributeDescriptions.clear();
		pipelineConfig.bindingDescriptions.clear();
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;

		vulkPipeline = make_unique<Pipeline>(
			vulkDevice,
			shaders.vertSh,
			shaders.fragSh,
			pipelineConfig
		);
	}

	void PointLightSystem::onUpdate(RenderTask& renderTask, vector<VkDescriptorSet> descriptorSets, LightUBO& ubo) {
		int lightIndex = 0;
		map<float, VulkanObject::id_t> sorted;

		for (auto& entry : renderTask.objects) {
			auto& obj = entry.second;
			if (obj.pointLight == nullptr) continue;

			auto offset = renderTask.camera.getPos() - obj.transform.translation;
			float disSq = glm::dot(offset, offset);
			sorted[disSq] = obj.getID();

			lightIndex++;
		}
		ubo.pointLightCount = lightIndex;

		lightIndex = 0;
		for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
			auto& obj = renderTask.objects.at(it->second);
			ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
			ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
			ubo.pointLights[lightIndex].radius = obj.transform.scale.x;
			
			lightIndex++;
		}
		

	}

	void PointLightSystem::onRender(RenderTask& renderTask, vector<VkDescriptorSet> descriptorSets, LightUBO& ubo) {
		vulkPipeline->bind(renderTask.commandBuffer);

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

		vkCmdDraw(renderTask.commandBuffer, 6 * ubo.pointLightCount, 1, 0, 0);
	}

}