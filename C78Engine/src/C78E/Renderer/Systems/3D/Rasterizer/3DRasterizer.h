#pragma once
#include <C78E/Renderer/Systems/3D/3DRenderer.h>

namespace C78E {

	class Rasterizer3D : public Renderer3D {
	public:
		struct RenderPass {
			Ref<Shader> shader = nullptr;
			Ref<VertexBuffer> vertexBuffer = nullptr;
			Ref<IndexBuffer> indexBuffer = nullptr;
			Ref<VertexArray> vertexArray = nullptr;
			std::vector<Ref<Texture>> textureSlots{};
			std::vector<Ref<UniformBuffer>> uniformBuffers{};

			bool writeDepthBuffer = true;
			RendererAPI::DepthFunc depthFunc = RendererAPI::DepthFunc::LESS;
		};
	private:
		static uint32_t s_MaxTextureSlots;
	public:
		Rasterizer3D(Ref<AssetManager> assetManager);
		Rasterizer3D(const Rasterizer3D& other) = delete;
		~Rasterizer3D();

		virtual bool render(Ref<Scene> scene) override;

		virtual bool display() override;

		//virtual const bool isTargetReady() const;
		//virtual const bool invalidateTarget();
		//virtual Ref<Texture2D> getTargetTexture() const;

		//virtual FrameInfo getTargetInfo() const;

		virtual inline const RendererType getType() const override;

	protected:
		//virtual bool initTarget() final;
		//virtual void resetTargetFrameInfo();
	private:

	};

}
