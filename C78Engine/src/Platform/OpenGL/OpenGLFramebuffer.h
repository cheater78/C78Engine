#pragma once
#include <C78E/Renderer/API/Framebuffer.h>

namespace C78E {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void resize(uint32_t width, uint32_t height) override;
		virtual uint32_t readPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void clearAttachment(uint32_t attachmentIndex, int value) override;

		Ref<Texture2D> getColorAttachment(uint32_t id) override;

		virtual const FramebufferSpecification& getSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<Ref<Texture2D>> m_ColorAttachments;
		Ref<Texture2D> m_DepthAttachment = nullptr;
	};

}
