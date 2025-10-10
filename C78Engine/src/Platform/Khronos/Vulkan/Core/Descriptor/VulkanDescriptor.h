#pragma once
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	VkDescriptorSetLayoutBinding toVkDescriptorSetLayoutBinding(VkDescriptorType type, uint32_t binding, VkShaderStageFlags stages);

	class VulkanDescriptorPool {
	public:
		VulkanDescriptorPool(Ref<VulkanDevice> device, uint32_t maxSets, uint32_t descriptorCount);
		~VulkanDescriptorPool();

	public:

		// DescriptorSetLayout
		bool createDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout, const std::vector<VkDescriptorSetLayoutBinding>& layoutBindings) const;
		void destroyDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout) const;

		// DescriptorSet
		bool allocateDescriptorSets(VkDescriptorSet* pDescriptorSets, uint32_t setCount, const VkDescriptorSetLayout* pDescriptorSetLayouts);
		bool freeDescriptorSets(const VkDescriptorSet* pDescriptorSets, uint32_t setCount);


	protected:
		Ref<VulkanDevice> m_Device;
		VkDescriptorPool m_VkDescriptorPool = VK_NULL_HANDLE;
	};

}
