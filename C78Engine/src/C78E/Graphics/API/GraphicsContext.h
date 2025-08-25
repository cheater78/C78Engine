#pragma once
//#include <C78E/Renderer/API/SwapChain/SwapChain.h>
//#include <C78E/Renderer/API/Command/CommandBuffer.h>

//#include <C78E/Renderer/API/Buffer/VertexBuffer.h>
//#include <C78E/Renderer/API/Buffer/IndexBuffer.h>
//#include <C78E/Renderer/API/Buffer/UniformBuffer.h>
//#include <C78E/Renderer/API/Buffer/StorageBuffer.h>

namespace C78E {

	class Window;

	class GraphicsContext {
	public:
		static Scope<GraphicsContext> create(Window& window);
	public:
		GraphicsContext(Window& window);
		virtual ~GraphicsContext() = default;

		//virtual Ref<CommandBuffer> createCommandBuffer() = 0;
		//virtual bool submit(Ref<CommandBuffer> commandBuffer) = 0;
		//
		//virtual Ref<VertexBuffer> createVertexBuffer() = 0;
		//virtual Ref<IndexBuffer> createIndexBuffer() = 0;
		//virtual Ref<UniformBuffer> createUniformBuffer() = 0;
		//virtual Ref<StorageBuffer> createStorageBuffer() = 0;
		//
		//virtual void resize(ImageSize size) = 0;
		virtual void aquireNextSwapChainImage() = 0;
		
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
		//Ref<SwapChain> m_SwapChain = nullptr;

	};

}
