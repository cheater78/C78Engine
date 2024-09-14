#pragma once
#include <C78E/Renderer/Systems/3D/3DRenderer.h>

namespace C78E {

	class Rasterizer3D : public Renderer3D {
	public:
		struct RenderPass {
			Ref<Material> material = nullptr;
			std::vector <Ref<VertexBuffer>> vertexBuffers{};
			Ref<IndexBuffer> indexBuffer = nullptr;
			Ref<VertexArray> vertexArray = nullptr;
			std::vector<std::pair<AssetHandle, Ref<Texture>>> textureSlots{};
			std::vector<Ref<UniformBuffer>> uniformBuffers{};

			bool writeDepthBuffer = true;
			RendererAPI::DepthFunc depthFunc = RendererAPI::DepthFunc::LESS;
		};
		
		struct RenderScene {
			CameraUniform cameraUniform;
			std::vector<RenderPass> renderPasses;
			bool commit = false;

			void addCamUBOToCurrentRenderPass();
		};
	private:
		static uint32_t s_MaxTextureSlots;
	public:
		Rasterizer3D(Ref<AssetManager> assetManager);
		Rasterizer3D(const Rasterizer3D& other) = delete;
		~Rasterizer3D();

		virtual bool beginScene(Camera& camera, const glm::mat4& viewMatrix) override;
		virtual void submit(Ref<Scene> scene) override;
		virtual bool endScene() override;

		void render(Ref<RenderScene> currentScene);

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
		void submitSpriteComponents(C78E::Ref<Scene> scene);
		void submitTextComponents(C78E::Ref<Scene> scene);
		void submitModelComponents(C78E::Ref<Scene> scene);
		void submitSkyBoxComponents(C78E::Ref<Scene> scene);
	private:
		Ref<RenderScene> m_CurrentScene = nullptr;
	};

}
