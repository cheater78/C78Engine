#include "C78EPCH.h"
#include "3DRaytracer.h"

namespace C78E {

	Raytracer3D::Raytracer3D(Ref<AssetManager> assetManager)
		: Renderer3D(assetManager){
	}

	Raytracer3D::~Raytracer3D() {
	}

	bool Raytracer3D::beginScene(Camera& camera, const glm::mat4& viewMatrix)
	{
		return false;
	}

	void Raytracer3D::submit(Ref<Scene> scene)
	{
	}

	bool Raytracer3D::endScene()
	{
		return false;
	}

	

	bool Raytracer3D::display() {
		return false;
	}

	inline const Renderer::RendererType Raytracer3D::getType() const { return RendererType::Raytracer3D; }

}
