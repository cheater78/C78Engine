#pragma once
#include <C78E/Renderer/API/Texture.h>

namespace C78E {

	enum class FramebufferTextureFormat {
		None = 0,
		RGBA8,
		RED_INTEGER,
		DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format)
		{ }

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: attachments(attachments) 
		{ }

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification {
		uint32_t width = 0;
		uint32_t height = 0;
		FramebufferAttachmentSpecification attachments;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t readPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void clearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual const FramebufferSpecification& getOption() const = 0;

		virtual Ref<Texture2D> getColorAttachment(uint32_t id) = 0;

		static Ref<Framebuffer> create(const FramebufferSpecification& spec);
	};


}
