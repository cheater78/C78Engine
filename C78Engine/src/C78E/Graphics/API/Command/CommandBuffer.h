#pragma once
#include <C78E/Graphics/Core/GraphicsContextItem.h>
#include "RenderPass.h"

#include <C78E/Graphics/API/Pipeline/Pipeline.h>

#include <C78E/Graphics/API/Image/FrameBuffer.h>

#include <C78E/Graphics/API/Buffer/GPUBuffer.h>
#include <C78E/Graphics/API/Buffer/StagingBuffer.h>

#include <C78E/Graphics/API/Buffer/IndexBuffer.h>
#include <C78E/Graphics/API/Buffer/VertexBuffer.h>

//#include <C78E/Graphics/API/Buffer/UniformBuffer.h>
//#include <C78E/Graphics/API/Buffer/StorageBuffer.h>
//#include <C78E/Graphics/API/Buffer/PushConstant.h>

namespace C78E {

	/**
	 * @brief CommandBuffer Base
	 */
	class CommandBuffer : public virtual GraphicsContextItem {
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
		CommandBuffer(bool reusable = true, UsageFlags usage = Usage::Auto);
	public:
		virtual ~CommandBuffer();

		virtual void clear() = 0;
		virtual bool beginRecording() = 0;
		virtual bool endRecording() = 0;

		// General
		virtual void bindPipeline(Ref<Pipeline> pipeline) = 0;

		// Graphics Commands
		virtual void beginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frameBuffer) = 0;
		virtual void endRenderPass() = 0;
		
		virtual void bind(Ref<VertexBuffer> vertexBuffer) = 0;
		virtual void bind(Ref<IndexBuffer> indexBuffer) = 0;

		virtual void setRenderArea(const RenderArea& renderArea) = 0;
		virtual void drawVertices(size_t vertexCount, size_t instanceCount = 1) = 0;
		virtual void drawIndices(size_t indexCount, size_t instanceCount = 1) = 0;

		// Compute Commands

		// RayTrace Commands

		// Transfer Commands
		virtual void copyBuffer(
			GPUBuffer& srcBuffer,
			GPUBuffer& dstBuffer,
			size_t size,
			size_t srcOffset = 0,
			size_t dstOffset = 0) = 0;


	public:
		bool isReusable() const { return m_Reusable; }
		State getState() const { return m_State; }
	protected:
		void addUsage(UsageFlags usage);
	protected:
		bool m_Reusable = false; // If true, the command buffer can be reused after submission
		State m_State = State::Ready;
		UsageFlags m_Usage;

		std::vector<RenderPass> m_RenderPasses;
	};

}
