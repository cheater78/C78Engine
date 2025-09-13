#include "C78EPCH.h"
#include "VulkanCommandBuffer.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanRenderPass.h>
//
//#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipeline.h>
//
//#include <Platform/Khronos/Vulkan/API/Buffer/VulkanGPUBuffer.h>
//#include <Platform/Khronos/Vulkan/API/Buffer/VulkanVertexBuffer.h>
//#include <Platform/Khronos/Vulkan/API/Buffer/VulkanIndexBuffer.h>
//#include <Platform/Khronos/Vulkan/API/Buffer/VulkanUniformBuffer.h>
//#include <Platform/Khronos/Vulkan/API/Buffer/VulkanStorageBuffer.h>

namespace C78E {

	VulkanCommandBuffer::VulkanCommandBuffer(GraphicsContext& ctx, UsageFlags usage, VkCommandPool commandPool)
		: CommandBuffer(ctx, usage), m_VkQueueFlags(VK_QUEUE_TRANSFER_BIT), m_Pool(commandPool) {

		VulkanGraphicsContext& context = ctx.getAs<VulkanGraphicsContext>();
		m_Device = context.getDevice();

		if (usage & Usage::Graphics) {
			m_VkQueueFlags |= VK_QUEUE_GRAPHICS_BIT;
		}
		if (usage & Usage::Compute) {
			m_VkQueueFlags |= VK_QUEUE_COMPUTE_BIT;
		}
		if (usage & Usage::RayTrace) {
			m_VkQueueFlags |= VK_QUEUE_COMPUTE_BIT;
		}

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_Pool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VkResult commandBufferCreateResult = vkAllocateCommandBuffers(m_Device->getVkDevice(), &allocInfo, &m_CommandBuffer);
		C78E_ASSERT(commandBufferCreateResult == VK_SUCCESS, "VulkanCommandBuffer::VulkanCommandBuffer: failed to allocate command buffer!");

	}

	VulkanCommandBuffer::~VulkanCommandBuffer() {
		vkFreeCommandBuffers(m_Device->getVkDevice(), m_Pool, 1, &m_CommandBuffer);
	}
	
	bool VulkanCommandBuffer::beginRecording() {
		C78E_CORE_VALIDATE(m_State == State::Ready, return false,
			"VulkanCommandBuffer::beginRecording: CommandBuffer wasn't ready!");
		
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		VkResult beginCommandBufferResult = vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);
		C78E_CORE_VALIDATE(beginCommandBufferResult == VK_SUCCESS, return false,
			"VulkanCommandBuffer::beginRecording: failed to begin recording command buffer!");
		
		m_State = State::Recording;
		return true;
	}
	
	
	
	void VulkanCommandBuffer::beginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frameBuffer) {
		
		Ref<VulkanRenderPass> vulkanRenderPass = castRef<VulkanRenderPass>(renderPass);
		Ref<VulkanFrameBuffer> vulkanFrameBuffer = castRef<VulkanFrameBuffer>(frameBuffer);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulkanRenderPass->getVkRenderPass();
		renderPassInfo.framebuffer = vulkanFrameBuffer->getVkFrameBuffer();

		// TODO: not like this
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { vulkanFrameBuffer->getFrameBufferSpecification().size.x, vulkanFrameBuffer->getFrameBufferSpecification().size.y };

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} }; //TODO: rn fixed rgba, needs to be per RenderPass attachment
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(
			m_CommandBuffer,
			&renderPassInfo,
			VK_SUBPASS_CONTENTS_INLINE // Subpass contents
		);
	
	}

	void VulkanCommandBuffer::bindPipeline(Ref<Pipeline> pipeline) {
		Ref<VulkanPipeline> vulkanPipeline = castRef<VulkanPipeline>(pipeline);
		C78E_CORE_VALIDATE(vulkanPipeline, return, "VulkanCommandBuffer::bindPipeline: Pipeline is not of type VulkanPipeline!");
		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->getVkPipeline());
	}
	
	//
	//
	//Ref<Pipeline> VulkanCommandBuffer::createPipeline(Ref<PipelineLayout> layout) {
	//	Ref<Pipeline> pipeline;
	//
	//	switch (layout->getType()) {
	//	case PipelineType::Graphics:
	//		requiresGraphics();
	//		//TODO: hasCurrentRenderPass() check
	//		pipeline = createRef<VulkanPipeline>(m_GraphicsContext, getCurrentRenderPass(), layout);
	//
	//
	//		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	//
	//	}
	//
	//	
	//}
	//
	//RenderPass::RenderPass(CommandBuffer& commandBuffer, const RenderPassConfig& renderPassConfig)
	//	: m_CommandBuffer(commandBuffer) {
	//}
	//
	//void VulkanCommandBuffer::bind(Ref<VertexBuffer> vertexBuffer) {
	//	C78E_CORE_VALIDATE(vertexBuffer, return, "VulkanCommandBuffer::bind: VertexBuffer is null!");
	//	Ref<VulkanVertexBuffer> vulkanVertexBuffer = castRef<VulkanVertexBuffer>(vertexBuffer);
	//	C78E_CORE_VALIDATE(vulkanVertexBuffer, return, "VulkanCommandBuffer::bind: VertexBuffer is not of type VulkanVertexBuffer!");
	//
	//	vkCmdBindVertexBuffers(
	//		m_CommandBuffer,
	//		0, // First binding
	//		1, // Binding count
	//		vulkanVertexBuffer->getVkBufferPtr(), // Pointer to the vertex buffer handle
	//		nullptr // Offsets (optional, can be nullptr if no offset is needed)
	//	);
	//}
	//
	//void VulkanCommandBuffer::bind(Ref<IndexBuffer> indexBuffer) {
	//	vkCmdBindIndexBuffer(
	//		m_CommandBuffer,
	//		indexBuffer->getHandle(), // Index buffer handle
	//		0, // Offset in bytes
	//		indexBuffer->getIndexType() // Index type (VK_INDEX_TYPE_UINT16 or VK_INDEX_TYPE_UINT32)
	//	);
	//}
	//
	//void VulkanCommandBuffer::bind(Ref<UniformBuffer> uniformBuffer, uint32_t binding) {
	//	
	//
	//}
	//
	//void VulkanCommandBuffer::bind(Ref<StorageBuffer> storageBuffer, uint32_t binding) {
	//}
	//
	//void VulkanCommandBuffer::bind(Ref<PushConstant> pushConstant) {
	//	vkCmdPushConstants(
	//		m_CommandBuffer,
	//		pipelineLayout,
	//		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
	//		binding,
	//		data.size(),
	//		&data
	//	);
	//}
	//
	//void VulkanCommandBuffer::bind(Ref<DescriptorSet> descriptorSet) {
	//	vkCmdBindDescriptorSets(
	//		m_CommandBuffer,
	//		VK_PIPELINE_BIND_POINT_GRAPHICS, // Pipeline bind point
	//		pipelineLayout, // Pipeline layout
	//		0, // First set
	//		1, // Descriptor set count
	//		&frameInfo.globalDescriptorSet,
	//		0,
	//		nullptr
	//	);
	//}
	//
	//void VulkanCommandBuffer::draw() {
	//	vkCmdDrawIndexed(
	//		m_CommandBuffer,
	//		indexCount, // Number of indices to draw
	//		instanceCount, // Number of instances to draw
	//		firstIndex, // First index in the index buffer
	//		vertexOffset, // Vertex offset
	//		firstInstanceIndex // First instance index
	//	);
	//
	//	vkCmdDraw(
	//		m_CommandBuffer,
	//		vertexCount, // Number of vertices to draw
	//		instanceCount, // Number of instances to draw
	//		firstVertex, // First vertex index
	//		firstInstanceIndex // First instance index
	//	);
	//}
	//
	//


	void VulkanCommandBuffer::drawVertecies(size_t vertexCount, size_t instanceCount) {
		vkCmdDraw(m_CommandBuffer, vertexCount, instanceCount, 0, 0);
	}

	void VulkanCommandBuffer::endRenderPass() {
		vkCmdEndRenderPass(m_CommandBuffer);
	}
	
	bool VulkanCommandBuffer::endRecording() {
		C78E_CORE_VALIDATE(m_State == State::Recording, return false,
			"VulkanCommandBuffer::endRecording: CommandBuffer was not recording!");

		VkResult endCommandBufferResult = vkEndCommandBuffer(m_CommandBuffer);
		C78E_CORE_VALIDATE(endCommandBufferResult == VK_SUCCESS, return false, 
			"VulkanCommandBuffer::endRecording: failed to record command buffer!");

		m_State = State::Finalized;
		return true;
	}
	//
	//void VulkanCommandBuffer::copyBuffer(Ref<GPUBuffer> srcBuffer, Ref<GPUBuffer> dstBuffer, size_t size, size_t srcOffset, size_t dstOffset) {
	//	Ref<VulkanGPUBuffer> vulkanSrcBuffer = castRef<VulkanGPUBuffer>(srcBuffer);
	//	Ref<VulkanGPUBuffer> vulkanDstBuffer = castRef<VulkanGPUBuffer>(dstBuffer);
	//
	//	C78E_CORE_VALIDATE(m_Buffer != buffer.m_Buffer, return, "VulkanBuffer::copyFrom: Cannot copy from self! returning early..");
	//
	//	C78E_CORE_VALIDATE(srcOffset < buffer.getBufferSize(), return, "VulkanBuffer::copyFrom: Source offset is out of bounds!");
	//	C78E_CORE_VALIDATE(dstOffset < getBufferSize(), return, "VulkanBuffer::copyFrom: Destination offset is out of bounds!");
	//
	//	const size_t bufferIntersectionSize = glm::min(buffer.getBufferSize() - srcOffset, getBufferSize() - dstOffset);
	//	C78E_CORE_VALIDATE(bufferIntersectionSize > 0, return, "VulkanBuffer::copyFrom: No intersection between source and destination buffer!");
	//	C78E_CORE_VALIDATE(size == ULONG_MAX || size <= bufferIntersectionSize, return, "VulkanBuffer::copyFrom: Size greater than the buffers intersection size!");
	//	const size_t copySize = (size == ULONG_MAX) ? bufferIntersectionSize : size;
	//
	//	VkBufferCopy copyRegion{};
	//	copyRegion.srcOffset = srcOffset;
	//	copyRegion.dstOffset = dstOffset;
	//	copyRegion.size = size;
	//	vkCmdCopyBuffer(m_CommandBuffer, vulkanSrcBuffer->, dstBuffer, 1, &copyRegion);
	//}

	void VulkanCommandBuffer::clear() {

	}



	//VulkanCommandBufferManager::VulkanCommandBufferManager(GraphicsContext& graphicsContext)
	//	: CommandBufferManager(graphicsContext), m_Device(graphicsContext.getAs<VulkanGraphicsContext>().getDevice()) {
	//}
	//
	//void VulkanCommandBufferManager::createCommandPool(VkCommandPool& commandPool, uint32_t familyIndex) {
	//	VkCommandPoolCreateInfo poolInfo = {};
	//	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	//	poolInfo.queueFamilyIndex = familyIndex;
	//	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	//	VkResult result = vkCreateCommandPool(m_Device->getVkDevice(), &poolInfo, nullptr, &commandPool);
	//	C78E_CORE_ASSERT(result == VK_SUCCESS, "VulkanCommandBufferManager::createCommandPool: failed to create CommandPool!");
	//}
	//
	//void VulkanCommandBufferManager::destroyCommandPool(VkCommandPool& commandPool) {
	//	vkDestroyCommandPool(m_Device->getVkDevice(), commandPool, nullptr);
	//}
	//
	//VkCommandBuffer VulkanCommandBufferManager::beginSingleTimeCommands() {
	//	VkCommandBufferAllocateInfo allocInfo{};
	//	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	//	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	//	allocInfo.commandPool = commandPool;
	//	allocInfo.commandBufferCount = 1;
	//
	//	VkCommandBuffer commandBuffer;
	//	vkAllocateCommandBuffers(m_Device->getVkDevice(), &allocInfo, &commandBuffer);
	//
	//	VkCommandBufferBeginInfo beginInfo{};
	//	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//
	//	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	//	return commandBuffer;
	//}
	//
	//void VulkanCommandBufferManager::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
	//	vkEndCommandBuffer(commandBuffer);
	//
	//	VkSubmitInfo submitInfo{};
	//	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//	submitInfo.commandBufferCount = 1;
	//	submitInfo.pCommandBuffers = &commandBuffer;
	//
	//	vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE);
	//	vkQueueWaitIdle(graphicsQueue_);
	//
	//	vkFreeCommandBuffers(device_, commandPool, 1, &commandBuffer);
	//}

}


