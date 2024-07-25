#include "C78EPCH.h"
#include "Renderer.h"

namespace C78E {

	Renderer::Renderer(Ref<AssetManager> assetManager)
		: m_AssetManager(assetManager) {
	}

	Renderer::~Renderer() {

	}

	const bool Renderer::isTargetReady() const { return m_TargetReady; }
	const bool Renderer::invalidateTarget() { const bool prev = m_TargetReady; m_TargetReady = false; return prev; }
	Ref<Texture2D> Renderer::getTargetTexture() const {
		if (isTargetReady())
			return m_TargetFrame->getColorAttachment(0);
		return nullptr;
	}

	Renderer::FrameInfo Renderer::getTargetInfo() const { return m_TargetFrameInfo; }


	void Renderer::resetTargetFrameInfo() { m_TargetFrameInfo = FrameInfo(); }


	inline const Renderer::RendererType Renderer::getType() const { return Renderer::RendererType::None; }

}
