#include "C78EPCH.h"
#include "3DRasterizer.h"

namespace C78E {
	uint32_t Rasterizer3D::s_MaxTextureSlots = 0;

	Rasterizer3D::Rasterizer3D(Ref<AssetManager> assetManager)
		: Renderer3D(assetManager){
	}

	Rasterizer3D::~Rasterizer3D() {
	}

	bool Rasterizer3D::render(Ref<Scene> scene) {
		return false;
	}

	bool Rasterizer3D::display() {
		return false;
	}

	inline const Renderer::RendererType Rasterizer3D::getType() const { return Renderer::RendererType::Rasterizer3D; }

}
