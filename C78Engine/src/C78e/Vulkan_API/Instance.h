#pragma once

#include "window.h"
#include "device.h"
#include "renderer.h"
#include "swapChain.h"
#include "model.h"
#include "object.h"
#include "descriptors.h"


#include "systems/basicRenderSystem.h"
#include "systems/pointLightSystem.h"
#include "systems/textureRenderSystem.h"

#include <memory>
#include <vector>
#include <chrono>
#include "systems/mainRenderSystem.h"

using namespace std;

namespace C78e {

	class VKInstance {
	public:
		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;

		VKInstance(int width, int height, string title);
		~VKInstance();

		VKInstance(const VKInstance&) = delete;
		VKInstance& operator=(const VKInstance&) = delete;

		void run(Camera& camera, VulkanObject::Map& objects);

		GLFWwindow* getGLFWwindow() { return vulkWindow.getGLFWwindow(); }
		VkDevice getVkDevice() { return vulkDevice.device(); }

		bool stopped() { return vulkWindow.stopped(); }
		float getLastFrameTime() { return lastFrameTime; }

		void loadObjects(VulkanObject::Map& objects);

		void initDescr(string file);

	private:

		VulkanWindow vulkWindow;
		Device vulkDevice;

		MainRenderSystem vulkMainRenderer;

		//Renderer vulkRenderer;


		/*
		unique_ptr<DescriptorPool> globalPool{};
		
		vector<unique_ptr<Buffer>> uboBuffers;
		unique_ptr<DescriptorSetLayout> globalSetLayout;
		unique_ptr<DescriptorSetLayout> textureSetLayout;
		vector<VkDescriptorSet> globalDescriptorSets;
		vector<VkDescriptorSet> textureDescriptorSets;

		unique_ptr<TextureRenderSystem> texRenderSystem;
		unique_ptr<RenderSystem> renderSystem;
		unique_ptr <PointLightSystem> pointLightSystem;
		*/
		chrono::steady_clock::time_point currTime;
		float lastFrameTime = 0;
		
		unique_ptr<Texture> tex1;

		
	};
}