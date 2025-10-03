#pragma once
#include <C78E/Graphics/API/Command/CommandBuffer.h>
#include <Platform/Khronos/Vulkan/Core/VulkanGraphicsContextItem.h>
#include <Platform/Khronos/Vulkan/Core/Device/VulkanDevice.h>

namespace C78E {

	class VulkanCommandBuffer : public VulkanGraphicsContextItem, public CommandBuffer {
	public:
		VulkanCommandBuffer(GraphicsContext& ctx, bool reusable, UsageFlags usage, VkCommandPool commandPool);
		virtual ~VulkanCommandBuffer();

		virtual void clear() override;
		virtual bool beginRecording() override;
		virtual bool endRecording() override;

		// General
		virtual void bindPipeline(Ref<Pipeline> pipeline) override;

		// Graphics Commands
		virtual void beginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frameBuffer) override;
		virtual void endRenderPass() override;

		virtual void bind(Ref<VertexBuffer> vertexBuffer) override;
		virtual void bind(Ref<IndexBuffer> indexBuffer) override;

		virtual void setRenderArea(const RenderArea& renderArea) override;
		virtual void drawVertices(size_t vertexCount, size_t instanceCount = 1) override;
		virtual void drawIndices(size_t indexCount, size_t instanceCount = 1) override;
		
		// Compute Commands

		// RayTrace Commands

		// Transfer Commands
		virtual void copyBuffer(
			GPUBuffer& srcBuffer,
			GPUBuffer& dstBuffer,
			size_t size = 0,
			size_t srcOffset = 0,
			size_t dstOffset = 0) override;

		bool alive() override { return false; }
		void free() override { }

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
		VkQueueFlags m_VkQueueFlags; // TODO: convert on the fly from m_Usage

		bool m_HasSwapChainTarget = false;
	};
}
