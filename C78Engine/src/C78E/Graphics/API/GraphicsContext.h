#pragma once
#include "C78E/Core/Log/Log.h"
#include <C78E/Graphics/API/SwapChain/SwapChain.h>
#include <C78E/Graphics/API/Command/CommandBuffer.h>
#include <C78E/Graphics/API/Program/ShaderManager.h>

namespace C78E {

	class Window;

	class GraphicsContext {
	public:
		static Scope<GraphicsContext> create(Window& window);
	public:
		GraphicsContext(Window& window);
		virtual ~GraphicsContext() = default;

		virtual Ref<CommandBuffer> createCommandBuffer() = 0;
		virtual Scope<CommandBuffer> beginSingleTimeCommand(CommandBuffer::UsageFlags usage = CommandBuffer::Usage::Auto) = 0;

		// SwapChain
		bool hasSwapChain() const;
		SwapChain& createSwapChain(const SwapChainConfig& swapChainConfig);
		SwapChain& getSwapChain() const;
		
		// Shader Manager
		Ref<ShaderManager> createShaderManager(const FilePath& cacheDirectory);
		Ref<ShaderManager> getShaderManager() const;

		// General - TODO
		virtual bool submit(Ref<CommandBuffer> commandBuffer) = 0;

		// Graphics
		virtual uint32_t beginFrame() = 0;
		virtual bool submit(uint32_t frameIndex, Ref<CommandBuffer> commandBuffer) = 0;
		virtual bool submit(uint32_t frameIndex, const std::vector<Ref<CommandBuffer>>& commandBuffers) = 0;
		virtual bool endFrame(uint32_t frameIndex) = 0;
		
		// Transfer
		virtual bool copyBuffer(
			GPUBuffer& srcGPUBuffer,
			GPUBuffer& dstGPUBuffer,
			size_t size = 0, // 0 -> whole src size
			size_t srcOffset = 0,
			size_t dstOffset = 0) = 0;

	public:
		template <std::derived_from<GraphicsContext> T>
		inline T& getAs() {
			T* context = dynamic_cast<T*>(this);
			C78E_CORE_ASSERT(context, "GraphicsContext::getAs: Failed to cast GraphicsContext to Type T!");
			return *context;
		}
	protected:
		Window& m_Window;
		Ref<SwapChain> m_SwapChain = nullptr; //TODO: Scope?
		Ref<ShaderManager> m_ShaderManager = nullptr;
	};

}
