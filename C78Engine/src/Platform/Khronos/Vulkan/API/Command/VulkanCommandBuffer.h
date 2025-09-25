#pragma once
#include <C78E/Graphics/API/Command/CommandBuffer.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	class VulkanCommandBuffer : public CommandBuffer {
	public:
		VulkanCommandBuffer(GraphicsContext& ctx, UsageFlags usage, VkCommandPool commandPool);
		virtual ~VulkanCommandBuffer();

		virtual void clear() override;

		virtual bool beginRecording() override;
		virtual void beginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frameBuffer) override;

		virtual void bindPipeline(Ref<Pipeline> pipeline) override;

		virtual void setRenderArea(const RenderArea& renderArea) override;
		virtual void drawVertecies(size_t vertexCount, size_t instanceCount = 1) override;
		//
		virtual void endRenderPass() override;
		virtual bool endRecording() override;


	public:
		VkCommandBuffer getVkCommandBuffer() const { return m_VkCommandBuffer; }
		VkCommandBuffer* getVkCommandBufferPtr() { return &m_VkCommandBuffer; }
		VkQueueFlags getRequiredVkQueueFlags() const { return m_VkQueueFlags; }
		
		bool hasSwapChainTarget() const;
		uint32_t getSwapChainImageIndex() const;
	private:
		void requiresGraphics() { m_VkQueueFlags |= VK_QUEUE_GRAPHICS_BIT; }
		void requiresCompute() { m_VkQueueFlags |= VK_QUEUE_COMPUTE_BIT; }
		void requiresTransfer() { m_VkQueueFlags |= VK_QUEUE_TRANSFER_BIT; }
	private:
		Ref<VulkanDevice> m_Device;
		VkCommandPool m_Pool;
		VkCommandBuffer m_VkCommandBuffer = VK_NULL_HANDLE;
		VkQueueFlags m_VkQueueFlags;

		bool m_HasSwapChainTarget = false;
	};
}
