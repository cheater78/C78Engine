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

		


		// SwapChain
		SwapChain& createSwapChain(const SwapChainConfig& swapChainConfig);
		SwapChain& getSwapChain() const;


		// Shader Manager
		Ref<ShaderManager> createShaderManager(const FilePath& cacheDirectory) {
			C78E_CORE_TRACE("GraphicsContext::createSwapChain: creating ShaderManager...")
			return m_ShaderManager = createRef<ShaderManager>(*this, cacheDirectory);
		}
		Ref<ShaderManager> getShaderManager() {
			return m_ShaderManager;
		}


		// Render
		virtual uint32_t beginFrame() = 0;
		virtual bool submit(uint32_t frameIndex, Ref<CommandBuffer> commandBuffer) = 0;
		virtual bool submit(uint32_t frameIndex, const std::vector<Ref<CommandBuffer>>& commandBuffers) = 0;
		virtual bool endFrame(uint32_t frameIndex) = 0;

	public:

		template <typename T>
		requires std::derived_from<T, GraphicsContext>
		T& getAs() {
			T* context = dynamic_cast<T*>(this);
			C78E_CORE_ASSERT(context, "GraphicsContext::getAs: Failed to cast GraphicsContext to Type T!");
			return *context;
		}
	

	protected:
		Window& m_Window;
		Ref<SwapChain> m_SwapChain = nullptr;
		Ref<ShaderManager> m_ShaderManager = nullptr;
	};

}
