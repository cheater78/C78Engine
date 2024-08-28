#include "C78EPCH.h"
#include "Renderer.h"

namespace C78E {

	Renderer::Renderer(Ref<AssetManager> assetManager)
		: m_AssetManager(assetManager) {
		initTarget();
	}

	Renderer::~Renderer() {

	}

	const bool Renderer::isTargetReady() const { return true; }//
	const bool Renderer::invalidateTarget() { return true; }//
	Ref<Texture2D> Renderer::getTargetTexture() const {
		if (isTargetReady())
			return m_TargetFrame->getColorAttachment(0);
		return nullptr;
	}

	Renderer::FrameInfo Renderer::getTargetInfo() const { return m_TargetFrameInfo; }

	void Renderer::resizeTarget(uint32_t width, uint32_t height) {
		invalidateTarget();
		m_BusyTarget.acquire();
		m_TargetFrame->resize(width, height);
		m_BusyTarget.release();

		m_BusyWorking.acquire();
		m_WorkingFrame->resize(width, height);
		m_BusyWorking.release();
	}


	bool Renderer::initTarget() {
		FramebufferSpecification spec{
		1, 1,
		{ FramebufferTextureFormat::RGBA8 },
		1,
		true
		};

		m_TargetFrame = Framebuffer::create(spec);
		m_WorkingFrame = Framebuffer::create(spec);

		return true;
	}

	void Renderer::resetTargetFrameInfo() { m_TargetFrameInfo = FrameInfo(); }


	inline const Renderer::RendererType Renderer::getType() const { return Renderer::RendererType::None; }

}
