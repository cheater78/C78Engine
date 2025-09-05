#pragma once

//#include <C78E/Graphics/API/Pipeline/Pipeline.h>
//#include <C78E/Graphics/API/Buffer/VertexBuffer.h>
//#include <C78E/Graphics/API/Buffer/IndexBuffer.h>
//#include <C78E/Graphics/API/Buffer/UniformBuffer.h>
//#include <C78E/Graphics/API/Buffer/StorageBuffer.h>
//#include <C78E/Graphics/API/Buffer/FrameBuffer.h>

namespace C78E {

	class GraphicsContext;

	struct RenderPassAttachment {

		// clear value
	};

	struct RenderPassConfig {

		


	};

	class RenderPass {
	public:
		static Ref<RenderPass> create(GraphicsContext& ctx);
	protected:
		RenderPass(GraphicsContext& ctx);
	public:
		virtual ~RenderPass() = default;


		// create target framebuffer ? - or should be created outside and passed in
		// what about Swapchain images

	protected:
		GraphicsContext& m_GraphicsContext;
	};

} // namespace C78E::Renderer	
