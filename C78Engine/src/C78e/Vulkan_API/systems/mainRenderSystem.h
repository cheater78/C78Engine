#pragma once

#include "Vulkan_API/pipeline.h"
#include "Vulkan_API/device.h"
#include "Vulkan_API/model.h"
#include "Vulkan_API/object.h"
#include "Vulkan_API/camera.h"
#include "Vulkan_API/frameInfo.h"
#include <Vulkan_API/descriptors.h>
#include <Vulkan_API/renderer.h>

#include "basicRenderSystem.h"
#include "pointLightSystem.h"
#include "textureRenderSystem.h"

#include <memory>
#include <vector>

using namespace std;

namespace C78e {

	class MainRenderSystem {
	public:

		MainRenderSystem(Device& vulkDevice, VulkanWindow& vulkWindow, unsigned int frameCount);
		~MainRenderSystem();

		MainRenderSystem(const MainRenderSystem&) = delete;
		MainRenderSystem& operator=(const MainRenderSystem&) = delete;

		void render(Camera& camera, VulkanObject::Map& objects);

	private:

		Device& vulkDevice;
		Renderer vulkRenderer;

		void loadResources();

		unsigned int frameCount;
		unordered_map<unsigned int, ShaderSet> shaders;
		unordered_map<unsigned int, vector<unique_ptr<Buffer>>> buffers;
		unordered_map<unsigned int, unique_ptr<Texture>> textures;

		unsigned int addShader(string shader);
		unsigned int addShader(ShaderSet shader);
		unsigned int addBuffer(VkBufferUsageFlags usage, VkDeviceSize size);
		unsigned int addTexture(string texFile);

		unique_ptr<RenderSystem> baseSystem;
		unique_ptr<PointLightSystem> pointLightSystem;
		unique_ptr<TextureRenderSystem> textureSystem;

		unique_ptr<DescriptorPool> globalDescriptorPool;
		vector<unique_ptr<DescriptorSetLayout>> descSetLayouts;
		vector<VkDescriptorSet> descSets;



		unique_ptr<DescriptorWriter> texDescWriter;
	};
}