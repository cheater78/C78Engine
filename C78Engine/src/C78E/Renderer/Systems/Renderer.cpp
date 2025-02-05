#include "C78EPCH.h"
#include "Renderer.h"

namespace C78E {

	Renderer::Renderer(Ref<AssetManager> assetManager)
		: m_AssetManager(assetManager) {
		initTarget();
	}

	Renderer::~Renderer() {
		if(m_RenderThread)
			m_RenderThread->join();
	}

	bool Renderer::display() {
		return false;
	}

	const bool Renderer::invalidateTarget() { return true; }//
	Ref<Texture2D> Renderer::getTargetTexture() {
		if(m_TargetFrame && m_TargetFrame->getOption().attachments.attachments.size())
			return m_TargetFrame->getColorAttachment(0);
		return nullptr;
	}

	Renderer::FrameInfo Renderer::getTargetInfo() const { return m_TargetFrameInfo; }

	void Renderer::resizeTarget(uint32_t width, uint32_t height) {
		invalidateTarget();
		m_BusyTarget.lock();
		m_TargetFrame->resize(width, height);
		m_BusyTarget.unlock();

		m_BusyWorking.lock();
		m_WorkingFrame->resize(width, height);
		m_BusyWorking.unlock();
	}


	bool Renderer::initTarget() {
		FramebufferSpecification spec{
		1, 1,
		{ FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 },
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
