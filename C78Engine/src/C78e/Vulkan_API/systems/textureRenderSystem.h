#pragma once

#include "../pipeline.h"
#include "../device.h"
#include "../model.h"
#include "../object.h"
#include "../camera.h"
#include "../frameInfo.h"
#include <Vulkan_API/descriptors.h>
#include "basicRenderSystem.h"

#include <memory>
#include <vector>

using namespace std;

namespace C78e {

	class TextureRenderSystem {
	public:

		TextureRenderSystem(Device& vulkDevice, VkRenderPass renderPass, ShaderSet shaders, vector<VkDescriptorSetLayout>& descriptorSetLayouts);
		~TextureRenderSystem();

		TextureRenderSystem(const TextureRenderSystem&) = delete;
		TextureRenderSystem& operator=(const TextureRenderSystem&) = delete;

		void onUpdate(RenderTask& renderTask, vector<VkDescriptorSet> descriptorSets);

		void onRender(RenderTask& renderTask, vector<VkDescriptorSet>& descriptorSets, unique_ptr<DescriptorWriter>& textureDescWriter, unordered_map<unsigned int, unique_ptr<Texture>>& textures);

	private:

		Device& vulkDevice;
		VkPipelineLayout pipelineLayout;
		unique_ptr<Pipeline> vulkPipeline;

		ShaderSet shaders;

		void createPipelineLayout(vector<VkDescriptorSetLayout> descriptorSetLayouts);
		void createPipeline(VkRenderPass renderPass);


	};
}