#pragma once
#include "vulkanWindow.h"
#include "vulkanDevice.h"
#include "vulkanRenderer.h"
#include "vulkanDescriptors.h"
#include "types.h"


#include <string>
#include <filesystem>



#include "systems/basicRenderSystem.h"

namespace C78e {
	class VulkanInstance {
	public:

		VulkanInstance(const uint32_t& width, const uint32_t& height, std::string title);

		bool render(Scene& scene);

		VulkanWindow& getWindow() { return m_Window; }

		void compileShaders(std::string path);

		operator bool() { return m_IsAlive; }

	private:
		VulkanWindow m_Window;
		VulkanDevice m_Device;
		VulkanRenderer m_Renderer;

		bool m_IsAlive = true;



		unique_ptr<RenderSystem> baseSystem;

		std::list<std::pair<uint64_t, VulkanModel>> models{};

		unsigned int frameCount;
		unordered_map<unsigned int, ShaderSet> m_Shaders;
		unordered_map<unsigned int, vector<unique_ptr<VulkanBuffer>>> m_Buffers;

		unique_ptr<VulkanDescriptorPool> m_GlobalDescriptorPool;
		vector<unique_ptr<VulkanDescriptorSetLayout>> m_DescSetLayouts;
		vector<VkDescriptorSet> m_DescSets;

		unsigned int addShader(string shader);
		unsigned int addShader(ShaderSet shader);
		unsigned int addBuffer(VkBufferUsageFlags usage, VkDeviceSize size);
		//unsigned int addTexture(string texFile);

		void loadResources();
	};
}