#include "C78EPCH.h"
#include "3DRenderer.h"

namespace C78E {



	Renderer3D::Renderer3D(Ref<AssetManager> assetManager)
		: Renderer(assetManager) {
	}

	Renderer3D::~Renderer3D() {
	}

	inline const Renderer::RendererType Renderer3D::getType() const { return RendererType::None; }


}
