#pragma once

#include "RenderPass.h"

#include <C78E/Graphics/API/Buffer/FrameBuffer.h>
#include <C78E/Graphics/API/Pipeline/Pipeline.h>
//#include <C78E/Graphics/API/Buffer/VertexBuffer.h>
//#include <C78E/Graphics/API/Buffer/IndexBuffer.h>
//#include <C78E/Graphics/API/Buffer/UniformBuffer.h>
//#include <C78E/Graphics/API/Buffer/StorageBuffer.h>
//#include <C78E/Graphics/API/Buffer/PushConstant.h>

namespace C78E {

	class GraphicsContext;
	class CommandBufferManager;

	/**
	 * @brief CommandBuffer Base
	 */
	class CommandBuffer {
	public:
		enum class State : uint8_t {
			Ready = 0,
			Recording,
			Finalized,
			Submitted,
			Executing,
			Executed
		};
		enum Usage : uint8_t {
			Auto = 0,
			Graphics =	BIT(0),
			Compute =	BIT(1),
			RayTrace =	BIT(2),
			Control =	BIT(3),
		};
		using UsageFlags = uint8_t;
	protected:
		CommandBuffer(GraphicsContext& ctx, UsageFlags usage);
	public:
		virtual ~CommandBuffer();

		virtual bool isReusable() const final { return m_Reusable; }
		virtual State getState() const final { return m_State; }

		virtual bool beginRecording() = 0;
		virtual void beginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frameBuffer) = 0;
		
		virtual void bindPipeline(Ref<Pipeline> pipeline) = 0;
		//
		//virtual void bind(Ref<VertexBuffer> vertexBuffer) = 0; // currently in order
		//virtual void bind(Ref<IndexBuffer> indexBuffer) = 0;
		//virtual void bind(Ref<UniformBuffer> uniformBuffer, uint32_t binding) = 0;
		//virtual void bind(Ref<StorageBuffer> storageBuffer, uint32_t binding) = 0;
		//
		//virtual void bind(Ref<PushConstant> pushConstant) = 0;
		//virtual void bind(Ref<Texture> texure, uint32_t binding) = 0;
		//
		virtual void setRenderArea(const RenderArea& renderArea) = 0;
		virtual void drawVertecies(size_t vertexCount, size_t instanceCount = 1) = 0;
		//
		virtual void endRenderPass() = 0;
		virtual bool endRecording() = 0;
		//
		//virtual void copyBuffer(
		//	Ref<GPUBuffer> srcBuffer,
		//	Ref<GPUBuffer> dstBuffer,
		//	size_t size,
		//	size_t srcOffset = 0,
		//	size_t dstOffset = 0) = 0;

		virtual void clear() = 0;

	public:
		GraphicsContext& getGraphicsContext() const {
			return m_GraphicsContext;
		}
	protected:
		void addUsage(UsageFlags usage);
	protected:
		GraphicsContext& m_GraphicsContext;
		bool m_Reusable = false; // If true, the command buffer can be reused after submission
		State m_State = State::Ready;
		UsageFlags m_Usage;

		std::vector<RenderPass> m_RenderPasses;
	};

}
