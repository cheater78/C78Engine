#pragma once
#include <C78E/Renderer/Systems/Renderer.h>

namespace C78E {

	/*
	* abstract 3D Renderer
	*/
	class Renderer3D : public Renderer {
	public:
		Renderer3D(Ref<AssetManager> assetManager);
		Renderer3D(const Renderer3D& other) = delete;
		~Renderer3D();

		virtual bool beginScene(Camera& camera, const glm::mat4& viewMatrix) override = 0;
		virtual void submit(Ref<Scene> scene) override = 0;
		virtual bool endScene() override = 0;

		virtual bool display() override = 0;

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
