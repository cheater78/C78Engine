#pragma once
#include "Vulkan_API/types.h"

#include <Vulkan_API/vulkanDevice.h>
#include <Vulkan_API/vulkanPipeline.h>

#include <memory>
#include <vector>
#include <string>


using namespace std;

namespace C78e {

	class RenderSystem {
	public:

		RenderSystem(VulkanDevice& vulkDevice, VkRenderPass renderPass, ShaderSet shaders, vector<VkDescriptorSetLayout>& descriptorSetLayouts);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void onUpdate(RenderTask& renderTask, vector<VkDescriptorSet> descriptorSets);

		void onRender(RenderTask& renderTask, vector<VkDescriptorSet> descriptorSets);

	private:

		VulkanDevice& m_Device;
		VkPipelineLayout m_PipelineLayout;
		unique_ptr<VulkanPipeline> m_Pipeline;

		ShaderSet shaders;
		
		void createPipelineLayout(vector<VkDescriptorSetLayout> descriptorSetLayouts);
		void createPipeline(VkRenderPass renderPass);


	};
}