#include "C78EPCH.h"
#include "VulkanDescriptor.h"

namespace C78E {

	VkDescriptorSetLayoutBinding toVkDescriptorSetLayoutBinding(VkDescriptorType type, uint32_t binding, VkShaderStageFlags stages) {
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = binding;
		uboLayoutBinding.descriptorType = type;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = stages;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		return uboLayoutBinding;
	}

	VulkanDescriptorPool::VulkanDescriptorPool(Ref<VulkanDevice> device, uint32_t maxSets, uint32_t descriptorCount)
		: m_Device(device) {

		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = descriptorCount;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = maxSets;

		VkResult createResult = vkCreateDescriptorPool(m_Device->getVkDevice(), &poolInfo, nullptr, &m_VkDescriptorPool);
	}

	VulkanDescriptorPool::~VulkanDescriptorPool() {
		vkDestroyDescriptorPool(m_Device->getVkDevice(), m_VkDescriptorPool, nullptr);
		m_VkDescriptorPool = VK_NULL_HANDLE;
	}

	// DescriptorSetLayout
	bool VulkanDescriptorPool::createDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout, const std::vector<VkDescriptorSetLayoutBinding>& layoutBindings) const {
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();

		VkResult result = vkCreateDescriptorSetLayout(m_Device->getVkDevice(), &layoutInfo, nullptr, &descriptorSetLayout);

		return true;
	}
	void VulkanDescriptorPool::destroyDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout) const {
		vkDestroyDescriptorSetLayout(m_Device->getVkDevice(), descriptorSetLayout, nullptr);
		descriptorSetLayout = VK_NULL_HANDLE;
	}

	// DescriptorSet
	bool VulkanDescriptorPool::allocateDescriptorSets(VkDescriptorSet* pDescriptorSets, uint32_t setCount, const VkDescriptorSetLayout* pDescriptorSetLayouts) {
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_VkDescriptorPool;
		allocInfo.descriptorSetCount = setCount;
		allocInfo.pSetLayouts = pDescriptorSetLayouts;

		VkResult allocResult = vkAllocateDescriptorSets(m_Device->getVkDevice(), &allocInfo, pDescriptorSets);
		C78E_CORE_VALIDATE(allocResult == VK_SUCCESS, return false,
			"VulkanDescriptorPool::allocateDescriptorSets: Failed to allocate DescriptorSets!");
		return true;
	}
	bool VulkanDescriptorPool::freeDescriptorSets(const VkDescriptorSet* pDescriptorSets, uint32_t setCount) {
		VkResult freeResult = vkFreeDescriptorSets(m_Device->getVkDevice(), m_VkDescriptorPool, setCount, pDescriptorSets);
		C78E_CORE_VALIDATE(freeResult == VK_SUCCESS, return false,
			"VulkanDescriptorPool::allocateDescriptorSets: Failed to free DescriptorSets!");
		return true;
	}

}
