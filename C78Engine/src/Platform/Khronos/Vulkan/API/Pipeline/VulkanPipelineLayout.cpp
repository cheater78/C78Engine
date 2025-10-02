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
	
	VulkanPipelineLayout::VulkanPipelineLayout(GraphicsContext& ctx)
		: m_GraphicsContext(ctx.getAs<VulkanGraphicsContext>()),
		m_Device(ctx.getAs<VulkanGraphicsContext>().getDevice()) {
	}

	VulkanPipelineLayout::~VulkanPipelineLayout() {
		if (m_Layout) {
			vkDestroyPipelineLayout(m_Device->getVkDevice(), m_Layout, nullptr);
		}
	}


	VulkanGraphicsPipelineLayout::VulkanGraphicsPipelineLayout(GraphicsContext& ctx)
		 : PipelineLayout(), GraphicsPipelineLayout(), VulkanPipelineLayout(ctx) {

		writePipelineLayoutInputLayout();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = VK_NULL_HANDLE;
		pipelineLayoutInfo.flags = 0;

		// Descriptor Set Layouts
		const uint32_t setLayoutCount = static_cast<uint32_t>(m_DescriptorSetLayouts.size());
		pipelineLayoutInfo.setLayoutCount = setLayoutCount;
		pipelineLayoutInfo.pSetLayouts = (setLayoutCount) ? m_DescriptorSetLayouts.data() : VK_NULL_HANDLE;

		// Push Constants
		const uint32_t pushConstantRangeCount = static_cast<uint32_t>(m_PushConstantRanges.size());
		pipelineLayoutInfo.pushConstantRangeCount = pushConstantRangeCount;
		pipelineLayoutInfo.pPushConstantRanges = (pushConstantRangeCount) ? m_PushConstantRanges.data() : VK_NULL_HANDLE;

		VkResult result = vkCreatePipelineLayout(m_Device->getVkDevice(), &pipelineLayoutInfo, nullptr, &m_Layout);
		C78E_CORE_SOFT_VALIDATE(result == VK_SUCCESS, "buildPipelineLayout: Failed to create PipelineLayout!");

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
	

	void VulkanGraphicsPipelineLayout::writePipelineLayoutInputLayout() {
		m_DescriptorSetLayouts.clear();
		m_PushConstantRanges.clear();
		/*
		// Descriptor Set Layouts
		// TODO: manage descriptor set layouts properly, this is just a placeholder
		for(const InstanceBufferLayout& layout : m_InstanceBufferLayouts) {
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = static_cast<uint32_t>(m_DescriptorSetLayouts.size());
			binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; // Assuming storage buffer for instance data
			binding.descriptorCount = 1;
			binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; // Adjust as needed
			binding.pImmutableSamplers = nullptr; // Optional
			m_DescriptorSetLayouts.emplace_back(binding);
		}
		for(const VertexBufferLayout& layout : m_VertexBufferLayouts) {
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = static_cast<uint32_t>(m_DescriptorSetLayouts.size());
			binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; // Assuming storage buffer for vertex data
			binding.descriptorCount = 1;
			binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; // Adjust as needed
			binding.pImmutableSamplers = nullptr; // Optional
			m_DescriptorSetLayouts.emplace_back(binding);
		}
		// Push Constants
		for(const auto& [stage, layout] : m_PushConstantLayouts) {
			VkShaderStageFlagBits stageFlags = toVkShaderStage(stage);
			size_t offset = 0;
			for(const UniformLayout& layout : layout) {
				VkPushConstantRange pushConstantRange{};
				pushConstantRange.stageFlags = stageFlags;
				pushConstantRange.offset = offset;
				pushConstantRange.size = layout.getStride();
				m_PushConstantRanges.emplace_back(pushConstantRange);
				offset += layout.getStride(); // Update offset for the next push constant range
			}
		}
		*/
	}
	void VulkanGraphicsPipelineLayout::writePipelineVertexInputLayout() {
		m_BindingDescriptions.clear();
		m_AttributeDescriptions.clear();
		/*
		// Buffer Binding Descriptions map to Buffer Layouts = #Instance Buffers + #Vertex Buffers
		m_BindingDescriptions.reserve(m_InstanceBufferLayouts.size() + m_VertexBufferLayouts.size());
		for(const BufferLayout& bufferLayout : m_InstanceBufferLayouts) { // instanced buffers
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = static_cast<uint32_t>(m_BindingDescriptions.size());
			bindingDescription.stride = bufferLayout.getStride();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
			m_BindingDescriptions.emplace_back(bindingDescription);
			for(const BufferElement& element : bufferLayout.getElements()) { // buffer elements
				VkVertexInputAttributeDescription attributeDescription{};
				attributeDescription.location = static_cast<uint32_t>(m_AttributeDescriptions.size());
				attributeDescription.binding = bindingDescription.binding; // Must match the binding in the vertex input binding description
				attributeDescription.format = toVkFormat(element.type); // Convert ShaderDataType to VkFormat
				attributeDescription.offset = element.offset; // Offset of the attribute in the vertex data
				m_AttributeDescriptions.emplace_back(attributeDescription);
			}
		}
		for(const BufferLayout& bufferLayout : m_VertexBufferLayouts) { // ordered buffers
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = static_cast<uint32_t>(m_BindingDescriptions.size());
			bindingDescription.stride = bufferLayout.getStride();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			m_BindingDescriptions.emplace_back(bindingDescription);

			for(const BufferElement& element : bufferLayout.getElements()) { // buffer elements
				VkVertexInputAttributeDescription attributeDescription{};
				attributeDescription.location = static_cast<uint32_t>(m_AttributeDescriptions.size());
				attributeDescription.binding = bindingDescription.binding; // Must match the binding in the vertex input binding description
				attributeDescription.format = toVkFormat(element.type); // Convert ShaderDataType to VkFormat
				attributeDescription.offset = element.offset; // Offset of the attribute in the vertex data
				m_AttributeDescriptions.emplace_back(attributeDescription);
			}
		}
		*/
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
