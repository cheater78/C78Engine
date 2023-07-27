#pragma once

#include "../pipeline.h"
#include "../device.h"
#include "../model.h"
#include "../object.h"
#include "../camera.h"
#include "../frameInfo.h"
#include "basicRenderSystem.h"

#include <memory>
#include <vector>

using namespace std;

namespace C78e {

	class PointLightSystem{
	public:

		PointLightSystem( Device& vulkDevice, VkRenderPass renderPass, ShaderSet shaders, vector<VkDescriptorSetLayout>& descriptorSetLayouts);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;


		void onUpdate(RenderTask& renderTask, vector<VkDescriptorSet> descriptorSets, LightUBO& ubo);

		void onRender(RenderTask& renderTask, vector<VkDescriptorSet> descriptorSets, LightUBO& ubo);

	private:

		Device& vulkDevice;
		VkPipelineLayout pipelineLayout;
		unique_ptr<Pipeline> vulkPipeline;

		ShaderSet shaders;

		void createPipelineLayout(vector<VkDescriptorSetLayout> descriptorSetLayouts);
		void createPipeline(VkRenderPass renderPass);


	};
}