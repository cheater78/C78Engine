#pragma once

#include "C78E/Renderer/API/Texture.h"

namespace C78E {

	static GLenum toGLDataFormat(Image::ImageFormat format) {
		switch (format) {
		case Image::ImageFormat::R8:		return GL_RED;
		case Image::ImageFormat::R32:		return GL_RED_INTEGER;
		case Image::ImageFormat::RG8:		return GL_RG;
		case Image::ImageFormat::RGB8:		return GL_RGB;
		case Image::ImageFormat::RGBA8:		return GL_RGBA;
		case Image::ImageFormat::RGBA32F:	return GL_RGBA;
		case Image::ImageFormat::D24S8:		return GL_DEPTH_STENCIL_ATTACHMENT;
		default: C78_CORE_ASSERT(false);	return 0;
		}
	}

	static GLenum toGLInternalFormat(Image::ImageFormat format) {
		switch (format) {
		case Image::ImageFormat::R8:		return GL_R8;
		case Image::ImageFormat::R32:		return GL_R32I;
		case Image::ImageFormat::RG8:		return GL_RG8;
		case Image::ImageFormat::RGB8:		return GL_RGB8;
		case Image::ImageFormat::RGBA8:		return GL_RGBA8;
		case Image::ImageFormat::RGBA32F:	return GL_RGBA32F;
		case Image::ImageFormat::D24S8:		return GL_DEPTH24_STENCIL8;
		default: C78_CORE_ASSERT(false);	return 0;
		}
	}

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const Texture2D::TextureSpecification& specification);
		OpenGLTexture2D(const Texture2D::TextureSpecification& specification, const Buffer& data);
		OpenGLTexture2D(const Image& image);
		OpenGLTexture2D(const OpenGLTexture2D&) = default; // Maybe needed!
		virtual ~OpenGLTexture2D();

		OpenGLTexture2D(const Texture2D::TextureSpecification& specification, uint32_t rendererID); // Do Not Use

		virtual uint32_t getWidth() const override { return m_Specification.width;  }
		virtual uint32_t getHeight() const override { return m_Specification.height; }
		virtual uint32_t getRendererID() const override { return m_RendererID; }
		
		virtual void setData(void* data, uint32_t size) override;
		virtual void bind(uint32_t slot = 0) const override;
		virtual void bindImage(uint32_t binding = 0) const override;

		virtual bool isLoaded() const override { return m_IsLoaded; }
		virtual const Texture2D::TextureSpecification& getSpecification() const override { return m_Specification; }

		virtual bool operator==(const Texture& other) const override { return m_RendererID == other.getRendererID(); }
	private:
		
	private:
		uint32_t m_RendererID;
		Texture2D::TextureSpecification m_Specification;

		bool m_IsLoaded = false;

	};


	class OpenGLCubeMap : public CubeMap {
	public:
		OpenGLCubeMap(CubeMap::TextureSpecification& specification);
		OpenGLCubeMap(std::vector<Image>& images);
		OpenGLCubeMap(Ref<Image> crossCubeMap);
		~OpenGLCubeMap();

		uint32_t getSize() const override { return m_Specification.size; };
		virtual uint32_t getRendererID() const override { return m_RendererID; };

		virtual void setData(void* data, uint32_t size) override;
		virtual void bind(uint32_t slot = 0) const override;

		virtual bool isLoaded() const override { return m_IsLoaded; };
		const CubeMap::TextureSpecification& getSpecification() const override { return m_Specification; };

		bool operator==(const Texture& other) const override { return m_RendererID == other.getRendererID(); }
	private:
		uint32_t m_RendererID = 0;
		CubeMap::TextureSpecification m_Specification;

		bool m_IsLoaded = false;
	};

}
