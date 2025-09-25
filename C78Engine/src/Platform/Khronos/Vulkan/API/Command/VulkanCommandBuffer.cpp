#include "C78EPCH.h"
#include "VulkanCommandBuffer.h"

#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanRenderPass.h>


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

		VkResult commandBufferCreateResult = vkAllocateCommandBuffers(m_Device->getVkDevice(), &allocInfo, &m_VkCommandBuffer);
		C78E_ASSERT(commandBufferCreateResult == VK_SUCCESS, "VulkanCommandBuffer::VulkanCommandBuffer: failed to allocate command buffer!");

	}

	VulkanCommandBuffer::~VulkanCommandBuffer() {
		vkFreeCommandBuffers(m_Device->getVkDevice(), m_Pool, 1, &m_VkCommandBuffer);
	}
	
	bool VulkanCommandBuffer::beginRecording() {
		//TODO: allow multiple begin/end recording cycles? - reset command buffer on beginRecording if needed
		//vkResetCommandBuffer(m_VkCommandBuffer, 0);
		C78E_CORE_VALIDATE(m_State == State::Ready, return false,
			"VulkanCommandBuffer::beginRecording: CommandBuffer wasn't ready!");
		
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		VkResult beginCommandBufferResult = vkBeginCommandBuffer(m_VkCommandBuffer, &beginInfo);
		C78E_CORE_VALIDATE(beginCommandBufferResult == VK_SUCCESS, return false,
			"VulkanCommandBuffer::beginRecording: failed to begin recording command buffer!");
		
		m_State = State::Recording;
		return true;
	}
	
	
	
	void VulkanCommandBuffer::beginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frameBuffer) {
		// Mark command buffer for Graphics usage
		requiresGraphics();

		// Does the command buffer target a swap chain image?
		m_HasSwapChainTarget |= frameBuffer->isSwapChainTarget();

		// Begin the render pass
		Ref<VulkanRenderPass> vulkanRenderPass = castRef<VulkanRenderPass>(renderPass);
		Ref<VulkanFrameBuffer> vulkanFrameBuffer = castRef<VulkanFrameBuffer>(frameBuffer);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulkanRenderPass->getVkRenderPass();
		renderPassInfo.framebuffer = vulkanFrameBuffer->getVkFrameBuffer();

		// TODO: not like this - RenderArea is a thing but should be reconsidered
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = toVkExtent(vulkanFrameBuffer->getSize());

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} }; //TODO: rn fixed rgba, needs to be per RenderPass attachment
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(
			m_VkCommandBuffer,
			&renderPassInfo,
			VK_SUBPASS_CONTENTS_INLINE // Subpass contents
		);
	
	}

	void VulkanCommandBuffer::bindPipeline(Ref<Pipeline> pipeline) {
		Ref<VulkanPipeline> vulkanPipeline = castRef<VulkanPipeline>(pipeline);
		C78E_CORE_VALIDATE(vulkanPipeline, return, "VulkanCommandBuffer::bindPipeline: Pipeline is not of type VulkanPipeline!");
		vkCmdBindPipeline(m_VkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->getVkPipeline());
	}
	
	void VulkanCommandBuffer::setRenderArea(const RenderArea& renderArea) {
		VkViewport viewport{};
		viewport.x = static_cast<float>(renderArea.viewport.offset.x);
		viewport.y = static_cast<float>(renderArea.viewport.offset.y);
		viewport.width = static_cast<float>(renderArea.viewport.size.x);
		viewport.height = static_cast<float>(renderArea.viewport.size.y);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_VkCommandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { static_cast<int32_t>(renderArea.scissor.offset.x), static_cast<int32_t>(renderArea.scissor.offset.y) };
		scissor.extent = { static_cast<uint32_t>(renderArea.scissor.size.x), static_cast<uint32_t>(renderArea.scissor.size.y) };
		vkCmdSetScissor(m_VkCommandBuffer, 0, 1, &scissor);
	}

	void VulkanCommandBuffer::drawVertecies(size_t vertexCount, size_t instanceCount) {
		vkCmdDraw(m_VkCommandBuffer, static_cast<uint32_t>(vertexCount), static_cast<uint32_t>(instanceCount), 0, 0);
	}

	void VulkanCommandBuffer::endRenderPass() {
		vkCmdEndRenderPass(m_VkCommandBuffer);
	}
	
	bool VulkanCommandBuffer::endRecording() {
		C78E_CORE_VALIDATE(m_State == State::Recording, return false,
			"VulkanCommandBuffer::endRecording: CommandBuffer was not recording!");

		VkResult endCommandBufferResult = vkEndCommandBuffer(m_VkCommandBuffer);
		C78E_CORE_VALIDATE(endCommandBufferResult == VK_SUCCESS, return false, 
			"VulkanCommandBuffer::endRecording: failed to record command buffer!");

		m_State = State::Finalized;
		return true;
	}

	bool VulkanCommandBuffer::hasSwapChainTarget() const { return m_HasSwapChainTarget; }

	void VulkanCommandBuffer::clear() {
		VkResult result = vkResetCommandBuffer(m_VkCommandBuffer, 0);
		C78E_CORE_SOFT_VALIDATE(result == VK_SUCCESS, "VulkanCommandBuffer::clear: failed to clear command buffer!");
	}


}


