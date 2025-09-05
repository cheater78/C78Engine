#pragma once
#include <C78E/Graphics/API/SwapChain/SwapChain.h>
#include <C78E/Graphics/API/Command/CommandBuffer.h>

//#include <C78E/Graphics/API/Buffer/VertexBuffer.h>
//#include <C78E/Graphics/API/Buffer/IndexBuffer.h>
//#include <C78E/Graphics/API/Buffer/UniformBuffer.h>
//#include <C78E/Graphics/API/Buffer/StorageBuffer.h>

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
		//virtual bool submit(Ref<CommandBuffer> commandBuffer) = 0;
		//
		//virtual Ref<VertexBuffer> createVertexBuffer() = 0;
		//virtual Ref<IndexBuffer> createIndexBuffer() = 0;
		//virtual Ref<UniformBuffer> createUniformBuffer() = 0;
		//virtual Ref<StorageBuffer> createStorageBuffer() = 0;
		//
		

		// SwapChain

		Ref<SwapChain> createSwapChain(const SwapChainConfig& swapChainConfig);
		Ref<SwapChain> getSwapChain() const;
		
		// Shader Manager
		Ref<ShaderManager> createShaderManager(const FilePath& cacheDirectory) {
			return m_ShaderManager = createRef<ShaderManager>(*this, cacheDirectory);
		}
		Ref<ShaderManager> getShaderManager() {
			return m_ShaderManager;
		}

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
