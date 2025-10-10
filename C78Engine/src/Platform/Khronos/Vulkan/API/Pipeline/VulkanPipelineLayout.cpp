#include "C78EPCH.h"
#include "VulkanPipelineLayout.h"

#include <C78E/Graphics/API/Buffer/BufferLayout.h>
#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>

namespace C78E {

	VkFormat toVkFormat(ShaderDataType type) {
		switch(type.raw()) {
		case PrimitiveType::Float32:
			switch (type.elementCount()) {
			case 1: return VK_FORMAT_R32_SFLOAT;
			case 2: return VK_FORMAT_R32G32_SFLOAT;
			case 3: return VK_FORMAT_R32G32B32_SFLOAT;
			case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
			default: 
				C78E_CORE_ERROR("toVkFormat: Unsupported number of elements for Float32 ShaderDataType! was {}", type.elementCount());
				return VK_FORMAT_R32_SFLOAT;
			}
		case PrimitiveType::Int32:
			switch (type.elementCount()) {
			case 1: return VK_FORMAT_R32_SINT;
			case 2: return VK_FORMAT_R32G32_SINT;
			case 3: return VK_FORMAT_R32G32B32_SINT;
			case 4: return VK_FORMAT_R32G32B32A32_SINT;
			default:
				C78E_CORE_ERROR("toVkFormat: Unsupported number of elements for Int32 ShaderDataType! was {}", type.elementCount());
				return VK_FORMAT_R32_SINT;
			}
		case PrimitiveType::UInt32:
			switch (type.elementCount()) {
			case 1: return VK_FORMAT_R32_UINT;
			case 2: return VK_FORMAT_R32G32_UINT;
			case 3: return VK_FORMAT_R32G32B32_UINT;
			case 4: return VK_FORMAT_R32G32B32A32_UINT;
			default:
				C78E_CORE_ERROR("toVkFormat: Unsupported number of elements for UInt32 ShaderDataType! was {}", type.elementCount());
				return VK_FORMAT_R32_UINT;
			}
		default:
			C78E_CORE_ASSERT(false, "toVkFormat: Unsupported ShaderDataType!");
		}
		return VkFormat();
	}
	
	VulkanPipelineLayout::VulkanPipelineLayout() {
	}

	VulkanPipelineLayout::~VulkanPipelineLayout() {
		//TODO: rly? idk 
		// if (m_VkPipelineLayout) {
		// 	vkDestroyPipelineLayout(m_Device->getVkDevice(), m_VkPipelineLayout, nullptr);
		// }
	}


	VulkanGraphicsPipelineLayout::VulkanGraphicsPipelineLayout(GraphicsContext& ctx)
		 : GraphicsContextItem(ctx), VulkanGraphicsContextItem(), PipelineLayout(), GraphicsPipelineLayout(), VulkanPipelineLayout() {
	}

	VulkanGraphicsPipelineLayout::~VulkanGraphicsPipelineLayout() {
		free();
	}

	std::vector<VkPipelineShaderStageCreateInfo> VulkanGraphicsPipelineLayout::getVkPipelineShaderStageCreateInfos() const {
		std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos;
		stageCreateInfos.reserve(m_Shaders.size());

		for(auto& [stage, shader] : m_Shaders) {
			stageCreateInfos.emplace_back(toVulkanPipelineShaderStageCreateInfo(stage, shader));
		}

		return stageCreateInfos;
	}

	VkPipelineVertexInputStateCreateInfo VulkanGraphicsPipelineLayout::getVertexInputInfo() {
		writePipelineVertexInputLayout();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pNext = nullptr;
		vertexInputInfo.flags = 0;
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(m_BindingDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = m_BindingDescriptions.data();
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_AttributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = m_AttributeDescriptions.data();
		return vertexInputInfo;
	}
	
	bool VulkanGraphicsPipelineLayout::init() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = VK_NULL_HANDLE;
		pipelineLayoutInfo.flags = 0;

		// Descriptor Set Layouts
		const bool descriptorSetLayoutsCreateResult = initDescriptorSetLayouts();
		C78E_CORE_VALIDATE(descriptorSetLayoutsCreateResult, return false,
			"VulkanGraphicsPipelineLayout::init: Failed to create DescriptorSetLayouts!");
		const uint32_t setLayoutCount = static_cast<uint32_t>(m_DescriptorSetLayouts.size());
		pipelineLayoutInfo.setLayoutCount = setLayoutCount;
		pipelineLayoutInfo.pSetLayouts = (setLayoutCount) ? m_DescriptorSetLayouts.data() : VK_NULL_HANDLE;

		// Push Constants
		const bool pushConstantRangesCreateResult = initPushConstantRanges();
		C78E_CORE_VALIDATE(pushConstantRangesCreateResult, return false,
			"VulkanGraphicsPipelineLayout::init: Failed to create PushConstantRanges!");
		const uint32_t pushConstantRangeCount = static_cast<uint32_t>(m_PushConstantRanges.size());
		pipelineLayoutInfo.pushConstantRangeCount = pushConstantRangeCount;
		pipelineLayoutInfo.pPushConstantRanges = (pushConstantRangeCount) ? m_PushConstantRanges.data() : VK_NULL_HANDLE;

		VkResult result = vkCreatePipelineLayout(m_Device->getVkDevice(), &pipelineLayoutInfo, nullptr, &m_VkPipelineLayout);
		C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "buildPipelineLayout: Failed to create PipelineLayout!");

		return true;
	}

	void VulkanGraphicsPipelineLayout::free() {
		if (m_VkPipelineLayout) {
			vkDestroyPipelineLayout(m_Device->getVkDevice(), m_VkPipelineLayout, nullptr);
			m_VkPipelineLayout = VK_NULL_HANDLE;
		}
		freeDescriptorSetLayouts();
		freePushConstantRanges();
	}

	bool VulkanGraphicsPipelineLayout::initDescriptorSetLayouts() {
		// Clean up all possibly alr existing DescriptorSetLayouts
		if (!m_DescriptorSetLayouts.empty()) {
			C78E_CORE_WARN("VulkanGraphicsPipelineLayout::initDescriptorSetLayouts: DescriptorSetLayouts alr existed! Removing them first...");
			freeDescriptorSetLayouts();
		}

		//TODO: add storageBuffers, etc. - all the things that use descriptorSets

		
		{ // Uniform Buffer - DrescriptorSetLayouts
			std::vector<VkDescriptorSetLayoutBinding> bindings;
			bindings.reserve(m_UniformBufferBindings.size());
			for (const BufferBinding& binding : m_UniformBufferBindings) {
				const VkDescriptorSetLayoutBinding layoutBinding = toVkDescriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					binding.binding,
					toVkShaderStage(binding.stages)
				);
				bindings.emplace_back(layoutBinding);
			}

			VkDescriptorSetLayout& uniformBufferLaout = m_DescriptorSetLayouts.emplace_back();

			// TODO: alr in VulkanDescriptor.h -> clean up
			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
			layoutInfo.pBindings = bindings.data();

			VkResult result = vkCreateDescriptorSetLayout(m_Device->getVkDevice(), &layoutInfo, nullptr, &uniformBufferLaout);
			C78E_CORE_VALIDATE(result == VK_SUCCESS, return false, "VulkanGraphicsPipelineLayout::initDescriptorSetLayouts: creating DescriptorSetLayout failed!");
		}

		return true;
	}

	void VulkanGraphicsPipelineLayout::freeDescriptorSetLayouts() {
		for (auto& descriptorSetLayout : m_DescriptorSetLayouts) {
			if (descriptorSetLayout) {
				vkDestroyDescriptorSetLayout(m_Device->getVkDevice(), descriptorSetLayout, nullptr);
			}
		}
		m_DescriptorSetLayouts.clear();
	}

	bool VulkanGraphicsPipelineLayout::initPushConstantRanges() {
		C78E_CORE_WARN("VulkanGraphicsPipelineLayout::initPushConstantRanges: is not impl! - pretending its fine...");
		return true;
	}

	void VulkanGraphicsPipelineLayout::freePushConstantRanges() {
	}

	void VulkanGraphicsPipelineLayout::writePipelineVertexInputLayout() {
		m_BindingDescriptions.clear();
		m_AttributeDescriptions.clear();
		
		// Buffer Binding Descriptions map to Buffer Layouts = #Instance Buffers + #Vertex Buffers
		m_BindingDescriptions.reserve(m_InstanceBufferLayouts.size() + m_VertexBufferLayouts.size());
		for(const VertexLayout& layout : m_InstanceBufferLayouts) { // instanced buffers
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = static_cast<uint32_t>(m_BindingDescriptions.size());
			bindingDescription.stride = layout.getStride();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
			m_BindingDescriptions.emplace_back(bindingDescription);

			uint32_t offset = 0;
			for(const VertexAttribute& element : layout.attributes()) { // buffer elements
				VkVertexInputAttributeDescription attributeDescription{};
				attributeDescription.location = static_cast<uint32_t>(m_AttributeDescriptions.size());
				attributeDescription.binding = bindingDescription.binding; // Must match the binding in the vertex input binding description
				attributeDescription.format = toVkFormat(element); // Convert ShaderDataType to VkFormat
				attributeDescription.offset = offset; // Offset of the attribute in the vertex data
				m_AttributeDescriptions.emplace_back(attributeDescription);
				offset += element.size();
			}
		}
		for(const VertexLayout& bufferLayout : m_VertexBufferLayouts) { // ordered buffers
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = static_cast<uint32_t>(m_BindingDescriptions.size());
			bindingDescription.stride = bufferLayout.getStride();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			m_BindingDescriptions.emplace_back(bindingDescription);

			uint32_t offset = 0;
			for(const VertexAttribute& element : bufferLayout.attributes()) { // buffer elements
				VkVertexInputAttributeDescription attributeDescription{};
				attributeDescription.location = static_cast<uint32_t>(m_AttributeDescriptions.size());
				attributeDescription.binding = bindingDescription.binding; // Must match the binding in the vertex input binding description
				attributeDescription.format = toVkFormat(element); // Convert ShaderDataType to VkFormat
				attributeDescription.offset = offset; // Offset of the attribute in the vertex data
				m_AttributeDescriptions.emplace_back(attributeDescription);
				offset += element.size();
			}
		}
		
	}

	std::vector<VkPipelineShaderStageCreateInfo> VulkanComputePipelineLayout::getVkPipelineShaderStageCreateInfos() const {
		std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos(1);
		stageCreateInfos[0] = toVulkanPipelineShaderStageCreateInfo(ShaderStage::Compute, m_Shader);
		return stageCreateInfos;
	}

	std::vector<VkPipelineShaderStageCreateInfo> VulkanRayTracingPipelineLayout::getVkPipelineShaderStageCreateInfos() const {
		std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos;
		stageCreateInfos.reserve(m_Shaders.size());

		for (auto& [stage, shader] : m_Shaders) {
			stageCreateInfos.emplace_back(toVulkanPipelineShaderStageCreateInfo(stage, shader));
		}

		return stageCreateInfos;
	}

}
