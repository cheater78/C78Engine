#include "mainRenderSystem.h"

#include <memory>
#include <iostream>
#include <Vulkan_API/vulkanBuffer.h>

using namespace std;

namespace C78e {

	MainRenderSystem::MainRenderSystem(VulkanDevice& vulkDevice, VulkanWindow& vulkWindow, unsigned int frameCount)
		: vulkDevice{ vulkDevice }, frameCount{ frameCount }, vulkRenderer{ vulkWindow, vulkDevice } {

		loadResources();

		vector<VkDescriptorSetLayout> layouts; 
		layouts = { descSetLayouts[0]->getDescriptorSetLayout(), descSetLayouts[1]->getDescriptorSetLayout() };
		baseSystem			= make_unique<RenderSystem>				(vulkDevice, vulkRenderer.getSwapChainRenderPass(), shaders[0], layouts);
		/*
		layouts = { descSetLayouts[0]->getDescriptorSetLayout(), descSetLayouts[1]->getDescriptorSetLayout() };
		pointLightSystem	= make_unique<PointLightSystem>			(vulkDevice, vulkRenderer.getSwapChainRenderPass(), shaders[1], layouts);
		layouts = { descSetLayouts[0]->getDescriptorSetLayout(), descSetLayouts[1]->getDescriptorSetLayout(), descSetLayouts[2]->getDescriptorSetLayout() };
		textureSystem		= make_unique<TextureRenderSystem>		(vulkDevice, vulkRenderer.getSwapChainRenderPass(), shaders[2], layouts);
		*/
	}

	MainRenderSystem::~MainRenderSystem() {
		
	}


	void MainRenderSystem::render(Camera& camera, VulkanObject::Map& objects) {

		camera.setViewYXZ();

		float aspect = vulkRenderer.getAscpectRatio();
		camera.setPerspectiveProj(glm::radians(80.f), aspect, 0.001f, 4047.f);

		if (auto commandBuffer = vulkRenderer.beginFrame()) {
			int frameIndex = vulkRenderer.getFrameIndex();

			RenderTask renderTask{
				frameIndex,
				commandBuffer,
				camera,
				objects
			};

			vector<VkDescriptorSet> baseSets = {descSets[0 + frameIndex], descSets[frameCount + frameIndex]};
			vector<VkDescriptorSet> pointSets = { descSets[0 + frameIndex], descSets[frameCount + frameIndex] };
			vector<VkDescriptorSet> texSets = { descSets[0 + frameIndex], descSets[frameCount + frameIndex], descSets[buffers.size()*frameCount] };

			//update
			GlobalUBO globalUBO{};
			globalUBO.projMat = camera.getProj();
			globalUBO.viewMat = camera.getView();
			globalUBO.invViewMat = camera.getInvView();

			LightUBO lightUBO{};

			baseSystem->onUpdate(renderTask, baseSets);
			pointLightSystem->onUpdate(renderTask, pointSets, lightUBO);
			textureSystem->onUpdate(renderTask, texSets);

			buffers[0][frameIndex]->writeToBuffer(&globalUBO);
			buffers[1][frameIndex]->writeToBuffer(&lightUBO);

			//render
			vulkRenderer.beginSwapChainRenderPass(commandBuffer);

			baseSystem->onRender(renderTask, baseSets);
			pointLightSystem->onRender(renderTask, pointSets, lightUBO);

			// dynamic textures
			textureSystem->onRender(renderTask, texSets, texDescWriter, textures);

			vulkRenderer.endSwapChainRenderPass(commandBuffer);
			vulkRenderer.endFrame();
		}
		vkDeviceWaitIdle(vulkDevice.device());
	}


	void MainRenderSystem::loadResources() {
		string shaderDir = "../res/shader/Vulkan/bin/";
		string texDir = "../res/textures/";

		addShader(shaderDir + "ezShader.glsl");
		addShader(shaderDir + "pointLight.glsl");
		addShader(shaderDir + "textureShader.glsl");

		addBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(GlobalUBO));
		addBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(LightUBO));

		addTexture(texDir + "Top.png");
		addTexture(texDir + "Side.png");
		addTexture(texDir + "Bot.png");

		uint32_t descSetCount = (static_cast<uint32_t>(buffers.size()) * frameCount) + static_cast<uint32_t>(textures.size());

		auto descPoolbuilder = DescriptorPool::Builder(vulkDevice)
			.setMaxSets(descSetCount);

		for (uint32_t i = 0; i < buffers.size(); i++) {
			auto& bufferSet = buffers.find(i)->second;
			auto& buffer = bufferSet[0];

			VkDescriptorType descType;

			if (buffer->getUsageFlags() & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
				descType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			}
			else {
				assert(true);
				return;
			}

			descPoolbuilder.addPoolSize(descType, frameCount);

			unique_ptr<DescriptorSetLayout> descSetlayout = DescriptorSetLayout::Builder(vulkDevice)
				.addBinding(0, descType, VK_SHADER_STAGE_ALL_GRAPHICS)
				.build();
			descSetLayouts.push_back(move(descSetlayout));
		}

		if (textures.size() > 0) {
			DescriptorSetLayout::Builder descSetlayout(vulkDevice);
			descPoolbuilder.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1);
			descSetlayout.addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL_GRAPHICS);
			descSetLayouts.push_back(descSetlayout.build());
		}
		else assert(true);
		globalDescriptorPool = descPoolbuilder.build();

		//  NOTE DescPOOL and DescLayouts DONE - 1 Texture fixed!!!
		
		for (uint32_t i = 0; i < buffers.size(); i++) {
			auto& bufferSet = buffers.find(i)->second;

			for (auto& buf : bufferSet) {
				VkDescriptorSet descriptorSet;
				assert(DescriptorWriter(*descSetLayouts[i], *globalDescriptorPool)
					.writeBuffer(0, &buf->descriptorInfo())
					.build(descriptorSet));
				descSets.push_back(move(descriptorSet));
			}

		}

		if (textures.size() > 0) {
			texDescWriter = make_unique<DescriptorWriter>(*descSetLayouts[buffers.size()], *globalDescriptorPool);
			texDescWriter->writeImage(0, &textures[0]->textureImageInfo);
			VkDescriptorSet descriptorSet;
			texDescWriter->build(descriptorSet);
			descSets.push_back(move(descriptorSet));
		}

		cout << "DescLayouts: " << descSetLayouts.size() << endl;
		cout << "DescSets: " << descSets.size() << endl;
		
		cout << "Shaders: " << shaders.size() << endl;
		cout << "Buffers: " << buffers.size() << endl;
		cout << "Textures: " << textures.size() << endl;
	}


	unsigned int MainRenderSystem::addShader(string shader) {
		return addShader( ShaderSet{ shader + ".vert.spv", shader + ".frag.spv" } );
	}

	unsigned int MainRenderSystem::addShader(ShaderSet shader) {
		static unsigned int shaderID = 0;
		shaders.emplace(shaderID, shader);
		cout << "Sha[" << shaderID << "] loaded: " << shader.vertSh << " / " << shader.fragSh << endl;
		return shaderID++;
	}

	unsigned int MainRenderSystem::addBuffer(VkBufferUsageFlags usage, VkDeviceSize size) {
		static unsigned int bufferID = 0;
		
		vector<unique_ptr<VulkanBuffer>> bufferSet(frameCount);

		for (auto& buffer : bufferSet) {
			buffer = make_unique<Buffer>(
				vulkDevice,
				size,
				1,
				usage,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);
		}

		buffers.emplace(bufferID, move(bufferSet));
		for (auto& buffer : buffers.find(bufferID)->second) {
			buffer->map();
		}
		return bufferID++;
	}

	unsigned int MainRenderSystem::addTexture(string texFile) {
		static unsigned int texID = 0;
		textures.emplace(texID, make_unique<Texture>(vulkDevice, texFile));
		cout << "Tex[" << texID << "] loaded: " << texFile << endl;
		return texID++;
	}
}