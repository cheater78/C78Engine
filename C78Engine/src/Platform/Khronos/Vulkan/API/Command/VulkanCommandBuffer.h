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

		//
		//virtual void bind(Ref<VertexBuffer> vertexBuffer) override;
		//virtual void bind(Ref<IndexBuffer> indexBuffer) override;
		//virtual void bind(Ref<UniformBuffer> uniformBuffer, uint32_t binding) override;
		//virtual void bind(Ref<StorageBuffer> storageBuffer, uint32_t binding) override;
		//
		//virtual void bind(Ref<PushConstant> pushConstant) override;
		//virtual void bind(Ref<Texture> texure, uint32_t binding) override;
		//
		//virtual void draw() override;
		virtual void drawVertecies(size_t vertexCount, size_t instanceCount = 1) override;
		//
		virtual void endRenderPass() override;
		


		virtual bool endRecording() override;

		//// Compute
		//
		//// Transfer
		//virtual void copyBuffer(
		//	Ref<GPUBuffer> srcBuffer,
		//	Ref<GPUBuffer> dstBuffer,
		//	size_t size = SIZE_MAX,
		//	size_t srcOffset = 0,
		//	size_t dstOffset = 0) override;


	public:
		std::vector<VkFence> getAllDependecyVkFences() const {
			// Iterate all RenderPasses
			//   Iterate all SubPasses
			//     Interate all Resources
			//       If Resource is Fenced, aquire VkFence
			// return list
			return { }; 
		}

	public:
		VkCommandBuffer getVkCommandBuffer() const { return m_CommandBuffer; }
		VkCommandBuffer* getVkCommandBufferPtr() { return &m_CommandBuffer; }
		VkQueueFlags getRequiredVkQueueFlags() const { return m_VkQueueFlags; }


	private:
		void requiresGraphics() { m_VkQueueFlags |= VK_QUEUE_GRAPHICS_BIT; }
		void requiresCompute() { m_VkQueueFlags |= VK_QUEUE_COMPUTE_BIT; }
		void requiresTransfer() { m_VkQueueFlags |= VK_QUEUE_TRANSFER_BIT; }
	private:
		Ref<VulkanDevice> m_Device;
		VkCommandPool m_Pool;
		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
		VkQueueFlags m_VkQueueFlags;

	};

	//class VulkanCommandBufferManager : public CommandBufferManager {
	//public:
	//	VulkanCommandBufferManager(GraphicsContext& graphicsContext);
	//	virtual ~VulkanCommandBufferManager() = default;
	//
	//	virtual Scope<SingleTimeCommandBuffer> getSingleTimeCommandBuffer() override;
	//	virtual Ref<CommandBuffer> getReusableCommandBuffer() override;
	//	virtual inline bool submit(Ref<CommandBuffer> commandBuffer) override;
	//private:
	//	void createCommandPool(VkCommandPool& commandPool, uint32_t familyIndex);
	//	void destroyCommandPool(VkCommandPool& commandPool);
	//private:
	//	Ref<VulkanDevice> m_Device;
	//};


}
