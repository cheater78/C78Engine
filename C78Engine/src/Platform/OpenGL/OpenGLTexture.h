#pragma once

#include "C78E/Assets/Texture/Texture.h"

#include <glad.h>

namespace C78E {

	static GLenum toGLDataFormat(ImageFormat format) {
		switch (format) {
		case ImageFormat::R8:		return GL_RED;
		case ImageFormat::RG8:		return GL_RG;
		case ImageFormat::RGB8:		return GL_RGB;
		case ImageFormat::RGBA8:	return GL_RGBA;
		case ImageFormat::RGBA32F:	return GL_RGBA;
		default: C78_CORE_ASSERT(false); return 0;
		}
	}

	static GLenum toGLInternalFormat(ImageFormat format) {
		switch (format) {
		case ImageFormat::R8:		return GL_R8;
		case ImageFormat::RG8:		return GL_RG8;
		case ImageFormat::RGB8:		return GL_RGB8;
		case ImageFormat::RGBA8:	return GL_RGBA8;
		case ImageFormat::RGBA32F:	return GL_RGBA32F;
		default: C78_CORE_ASSERT(false); return 0;
		}
	}

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const Texture2D::TextureSpecification& specification);
		OpenGLTexture2D(const Texture2D::TextureSpecification& specification, uint32_t rendererID);
		OpenGLTexture2D(std::string filename);
		OpenGLTexture2D(RawImage& image);
		OpenGLTexture2D(const OpenGLTexture2D&) = default; // Maybe needed!
		virtual ~OpenGLTexture2D();

		virtual std::string getName() const override { return m_Name; }
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
		std::string m_Name;
		uint32_t m_RendererID;
		Texture2D::TextureSpecification m_Specification;

		bool m_IsLoaded = false;
	};


	class OpenGLCubeMap : public CubeMap {
	public:
		OpenGLCubeMap(CubeMap::TextureSpecification& specification);
		OpenGLCubeMap(std::vector<RawImage>& images);
		OpenGLCubeMap(Ref<RawImage> crossCubeMap);
		~OpenGLCubeMap();

		virtual std::string getName() const override { return m_Name; };
		uint32_t getSize() const override { return m_Specification.size; };
		virtual uint32_t getRendererID() const override { return m_RendererID; };

		virtual void setData(void* data, uint32_t size) override;
		virtual void bind(uint32_t slot = 0) const override;

		virtual bool isLoaded() const override { return m_IsLoaded; };
		const CubeMap::TextureSpecification& getSpecification() const override { return m_Specification; };

		bool operator==(const Texture& other) const override { return m_RendererID == other.getRendererID(); }
	private:
		std::string m_Name;
		uint32_t m_RendererID = 0;
		CubeMap::TextureSpecification m_Specification;

		bool m_IsLoaded = false;
	};

}
