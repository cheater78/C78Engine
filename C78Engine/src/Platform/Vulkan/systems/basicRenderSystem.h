#pragma once

#include "../vulkanPipeline.h"
#include "../vulkanDevice.h"
#include "../vulkanModel.h"
#include "../types.h"

#include <memory>
#include <string>
#include <list>

using namespace std;

namespace C78E {

	class RenderSystem {
	public:

		RenderSystem(VulkanDevice& vulkDevice, VkRenderPass renderPass, ShaderSet shaders, vector<VkDescriptorSetLayout>& descriptorSetLayouts);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void onUpdate(RenderTask& renderTask, vector<VkDescriptorSet&> descriptorSets);

		void onRender(RenderTask& renderTask, vector<VkDescriptorSet&> descriptorSets);

	private:

		VulkanDevice& vulkDevice;
		VkPipelineLayout pipelineLayout;
		unique_ptr<VulkanPipeline> vulkPipeline;

		ShaderSet shaders;
		
		void createPipelineLayout(vector<VkDescriptorSetLayout> descriptorSetLayouts);
		void createPipeline(VkRenderPass renderPass);


	};
}