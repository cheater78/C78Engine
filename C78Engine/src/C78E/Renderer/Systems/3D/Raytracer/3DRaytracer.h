#pragma once
#include <C78E/Renderer/Systems/3D/3DRenderer.h>

namespace C78E {

	class Raytracer3D : public Renderer3D {
	public:
		Raytracer3D(Ref<AssetManager> assetManager);
		Raytracer3D(const Raytracer3D& other) = delete;
		~Raytracer3D();

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
	};

}
