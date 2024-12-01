#include "C78EPCH.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad.h>

namespace C78E {

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils {

		static GLenum textureTarget(bool multisampled) {
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void createTextures(bool multisampled, uint32_t* outID, uint32_t count) {
			glCreateTextures(textureTarget(multisampled), count, outID);
		}

		static void bindTexture(bool multisampled, uint32_t id) {
			glBindTexture(textureTarget(multisampled), id);
		}

		static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			} else {
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisampled), id, 0);
		}

		static void attachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			} else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
		}

		static bool isDepthFormat(FramebufferTextureFormat format) {
			switch (format) {
				case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}
			return false;
		}

		static GLenum FBTextureFormatToGLFormat(FramebufferTextureFormat format) {
			switch (format) {
				case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
				case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			C78E_CORE_ASSERT(false, "FBTextureFormatToGLFormat: no such FramebufferTextureFormat!");
			return 0;
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec) {
		for (auto spec : m_Specification.attachments.attachments) {
			if (!Utils::isDepthFormat(spec.TextureFormat)) {
				m_ColorAttachmentSpecifications.emplace_back(spec);
			} else {
				m_DepthAttachmentSpecification = spec;
			}
		}
		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
		m_ColorAttachments.clear();
		m_DepthAttachment.reset();
	}

	void OpenGLFramebuffer::invalidate() {
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			m_ColorAttachments.clear();
			m_DepthAttachment.reset();
		}
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.samples > 1;

		// Attachments
		if (m_ColorAttachmentSpecifications.size()) {
			for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++) {
				uint32_t rID = 0;
				Utils::createTextures(multisample, &rID, 1);
				
				switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
					case FramebufferTextureFormat::RGBA8:
						{
							Texture2D::TextureSpecification spec{ m_Specification.width, m_Specification.height, Image::ImageFormat::RGBA8, false };
							m_ColorAttachments.push_back(Texture2D::create(spec, rID));
							Utils::bindTexture(multisample, rID);
							Utils::attachColorTexture(rID, m_Specification.samples, GL_RGBA8, GL_RGBA, m_Specification.width, m_Specification.height, static_cast<int>(i));
						}
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						{
							Texture2D::TextureSpecification spec{ m_Specification.width, m_Specification.height, Image::ImageFormat::R32, false };
							m_ColorAttachments.push_back(Texture2D::create(spec, rID));
							Utils::bindTexture(multisample, rID);
							Utils::attachColorTexture(rID, m_Specification.samples, GL_R32I, GL_RED_INTEGER, m_Specification.width, m_Specification.height, static_cast<int>(i));
						}
						break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
			uint32_t rID = 0;
			Utils::createTextures(multisample, &rID, 1);
			Texture2D::TextureSpecification spec{ m_Specification.width, m_Specification.height, Image::ImageFormat::D24S8 };
			m_DepthAttachment = Texture2D::create(spec, rID);
			Utils::bindTexture(multisample, rID);
			switch (m_DepthAttachmentSpecification.TextureFormat) {
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::attachDepthTexture(rID, m_Specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.width, m_Specification.height);
					break;
			}
		}

		if (m_ColorAttachments.size() > 1) {
			C78E_CORE_ASSERT(m_ColorAttachments.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(static_cast<int>(m_ColorAttachments.size()), buffers);
		} else if (m_ColorAttachments.empty()) {
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		C78E_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.width, m_Specification.height);
	}

	void OpenGLFramebuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) {
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
			C78E_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.width = width;
		m_Specification.height = height;
		
		invalidate();
	}

	uint32_t OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y) {
		C78E_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		uint32_t pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFramebuffer::clearAttachment(uint32_t attachmentIndex, int value) {
		C78E_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
		C78E_CORE_ASSERT(false, "Implement Texture clear!");
		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		C78E_CORE_ERROR("OpenGLFramebuffer::clearAttachment: line commented out, fix me!");
		//glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::FBTextureFormatToGLFormat(spec.TextureFormat), GL_INT, &value);
	}

	Ref<Texture2D> OpenGLFramebuffer::getColorAttachment(uint32_t id) {
		C78E_CORE_ASSERT(id < m_ColorAttachments.size(), "ColorAttachmentID is out of bounds!");
		return m_ColorAttachments.at(id);
	}

}
