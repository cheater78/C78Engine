#include "vulkanInstance.h"

#include "C78e/engine.h"
#include "types.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <filesystem>

namespace C78E {

	VulkanInstance::VulkanInstance(const uint32_t& width, const uint32_t& height, std::string title)
		: m_Window(width, height, title), m_Device(m_Window), m_Renderer(m_Window, m_Device)
	{
		loadResources();
		vector<VkDescriptorSetLayout> layouts;
		layouts = { m_DescSetLayouts[0]->getDescriptorSetLayout(), m_DescSetLayouts[1]->getDescriptorSetLayout() };
		baseSystem = make_unique<RenderSystem>(m_Device, m_Renderer.getSwapChainRenderPass(), m_Shaders[0], layouts);
	}

	bool VulkanInstance::render(Scene& scene)
	{
		/*
		if (!m_IsAlive) return m_IsAlive;

		Camera cam = scene.getActiveCam();
		TransformComponent& camTrans = cam.getComponent<TransformComponent>();
		CameraComponent& camComp = cam.getComponent<CameraComponent>();

		models.clear();
		auto& meshEnts = scene.reg().view<MeshComponent>();
		for (auto& enttity : meshEnts) {
			Entity entity(enttity, &scene);
			models.emplace_back(std::make_pair((uint64_t)entity.getUUID(), VulkanModel( m_Device, entity.getUUID(), vector<Vertex>(entity.getComponent<MeshComponent>().vertecies.data()), *&entity.getComponent<MeshComponent>().indecies)));
		}

		if (auto cmdBuffer = m_Renderer.beginFrame()) {
			int frameIndex = m_Renderer.getFrameIndex();

			RenderTask renderTask{ frameIndex, cmdBuffer, &scene, models };

			GlobalUBO globalUBO{ cam.getProjMat(), cam.getViewMat(), cam.getIViewMat() };

			LightUBO lightUBO{};
			lightUBO.ambientLightColor = {1.f, 1.f, 1.f, .05f};

			auto pLights = scene.reg().view<PointLightComponent>();
			for (auto& ent : pLights) {
				Entity entity{ent, &scene};
				auto& pLight = entity.getComponent<PointLightComponent>();
				auto& pLtrans = entity.getComponent<TransformComponent>();

				PointLight p{};
				p.position = glm::vec4(pLtrans.translation, 0);
				p.color = glm::vec4(pLight.color, pLight.intensity);
				lightUBO.pointLights[lightUBO.pointLightCount % MAX_POINT_LIGHTS] = p;
				lightUBO.pointLightCount++;
			}

			//baseSystem->onUpdate(renderTask, baseSets);

			m_Buffers[0][frameIndex]->writeToBuffer(&globalUBO);
			m_Buffers[1][frameIndex]->writeToBuffer(&lightUBO);

			//render
			m_Renderer.beginSwapChainRenderPass(cmdBuffer);

			

			baseSystem->onRender(renderTask, std::vector{ m_DescSets[0 + frameIndex], m_DescSets[VulkanSwapChain::MAX_FRAMES_IN_FLIGHT + frameIndex] });


			m_Renderer.endSwapChainRenderPass(cmdBuffer);
			m_Renderer.endFrame();

		}
		
		vkDeviceWaitIdle(m_Device.device());
		m_IsAlive = !glfwWindowShouldClose(m_Window.getGLFWwindow());
		*/
		return m_IsAlive;
	}

	unsigned int VulkanInstance::addShader(string shader) {
		return addShader(ShaderSet{ shader + ".vert.spv", shader + ".frag.spv" });
	}

	unsigned int VulkanInstance::addShader(ShaderSet shader) {
		static unsigned int shaderID = 0;
		m_Shaders.emplace(shaderID, shader);
		std::cout << "Sha[" << shaderID << "] loaded: " << shader.vertSh << " / " << shader.fragSh << std::endl;
		return shaderID++;
	}

	unsigned int VulkanInstance::addBuffer(VkBufferUsageFlags usage, VkDeviceSize size) {
		static unsigned int bufferID = 0;

		vector<unique_ptr<VulkanBuffer>> bufferSet(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (auto& buffer : bufferSet) {
			buffer = std::make_unique<VulkanBuffer>(
				m_Device,
				size,
				1,
				usage,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);
		}

		m_Buffers.emplace(bufferID, move(bufferSet));
		for (auto& buffer : m_Buffers.find(bufferID)->second) {
			buffer->map();
		}
		return bufferID++;
	}

	void VulkanInstance::loadResources() {
		/*
		const std::filesystem::path c_CWD = std::filesystem::current_path();
		const std::filesystem::path c_ROOT = c_CWD.parent_path();

		string shaderDir = c_ROOT.string() + "/res/shader/Vulkan/bin/";
		string texDir = c_ROOT.string() + "/res/textures/";

		addShader(shaderDir + "ezShader.glsl");
		addShader(shaderDir + "pointLight.glsl");
		addShader(shaderDir + "textureShader.glsl");

		addBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(GlobalUBO));
		addBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(LightUBO));

		uint32_t descSetCount = (static_cast<uint32_t>(m_Buffers.size()) * VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

		auto descPoolbuilder = VulkanDescriptorPool::Builder(m_Device)
			.setMaxSets(descSetCount);

		for (uint32_t i = 0; i < m_Buffers.size(); i++) {
			auto& bufferSet = m_Buffers.find(i)->second;
			auto& buffer = bufferSet[0];

			VkDescriptorType descType;

			if (buffer->getUsageFlags() & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
				descType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			}
			else {
				assert(true);
				return;
			}

			descPoolbuilder.addPoolSize(descType, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

			unique_ptr<VulkanDescriptorSetLayout> descSetlayout = VulkanDescriptorSetLayout::Builder(m_Device)
				.addBinding(0, descType, VK_SHADER_STAGE_ALL_GRAPHICS)
				.build();
			m_DescSetLayouts.push_back(move(descSetlayout));
		}

		m_GlobalDescriptorPool = descPoolbuilder.build();

		//  NOTE DescPOOL and DescLayouts DONE - 1 Texture fixed!!!

		for (uint32_t i = 0; i < m_Buffers.size(); i++) {
			auto& bufferSet = m_Buffers.find(i)->second;

			for (auto& buf : bufferSet) {
				VkDescriptorSet descriptorSet;
				assert(VulkanDescriptorWriter(*m_DescSetLayouts[i], *m_GlobalDescriptorPool)
					.writeBuffer(0, &buf->descriptorInfo())
					.build(descriptorSet));
				m_DescSets.push_back(move(descriptorSet));
			}

		}

		cout << "DescLayouts: " << m_DescSetLayouts.size() << endl;
		cout << "DescSets: " << m_DescSets.size() << endl;

		cout << "Shaders: " << m_Shaders.size() << endl;
		cout << "Buffers: " << m_Buffers.size() << endl;
		*/
	}



	void VulkanInstance::compileShaders(std::string path) {
		std::filesystem::path cwd = std::filesystem::current_path();
		std::filesystem::path root = cwd.parent_path();
		std::filesystem::path shaderPath(path);
		
		std::cout << "RootDir: " << root.string() << std::endl;
		std::cout << "ShaderDir: " << root.string() + shaderPath.string() << std::endl;


		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(root.string() + shaderPath.string())) {
			if (dirEntry.path().filename().extension().string() != ".vert" && dirEntry.path().filename().extension().string() != ".frag")
				continue;
#if defined(_WIN32)
			std::string cmd = "C:\\dev\\lib\\VulkanSDK\\Bin\\glslc.exe ";
#elif(__linux__)
			string cmd = "ERROR";
#endif
			cmd += dirEntry.path().string();
			cmd += " -o ";
			cmd += dirEntry.path().parent_path().string() + "/bin/" + dirEntry.path().filename().string() + ".spv";

			cout << "GLSL Compiling Shader: " << dirEntry.path().filename().string() << endl;

			system(cmd.c_str());
		}
	}
}