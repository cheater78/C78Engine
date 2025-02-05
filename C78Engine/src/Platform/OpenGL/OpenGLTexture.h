#pragma once

#include "C78E/Renderer/API/Texture.h"

namespace C78E {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const Image& image, const Texture2D::Option& option = Texture2D::Option());
		OpenGLTexture2D(const ScopedBuffer& textureData, const Texture2D::Specification& specification, const Texture2D::Option& option = Texture2D::Option());
		OpenGLTexture2D(TextureHandle rendererID, const Texture2D::Specification& specification);
		OpenGLTexture2D(const OpenGLTexture2D&) = delete;
		virtual ~OpenGLTexture2D();

		virtual void setData(const ScopedBuffer& dataBuffer) override;
		virtual void setData(const ScopedBuffer& dataBuffer, size_t offsetX, size_t offsetY, size_t sizeX, size_t sizeY) override;
		virtual void bind(uint32_t slot = 0) const override;
		virtual void bindImage(uint32_t binding = 0) const override;

		virtual TextureHandle getRendererID() const override { return m_RendererID; }
		
	private:
		GLuint m_RendererID;
	};


	class OpenGLCubeMap : public CubeMap {
	public:
		OpenGLCubeMap(const CubeImageData& cubeMapImageData, const CubeMap::Option& option = CubeMap::Option());
		OpenGLCubeMap(const Image& crossCubeMap, const CubeMap::Option& option = CubeMap::Option());
		OpenGLCubeMap(const ScopedBuffer& textureData, const CubeMap::Specification& specification, const CubeMap::Option& option = CubeMap::Option());
		OpenGLCubeMap(TextureHandle rendererID, const CubeMap::Specification& specification);
		~OpenGLCubeMap();

		virtual void setData(const ScopedBuffer& dataBuffer) override;
		virtual void setData(const ScopedBuffer& dataBuffer, Face face, size_t offsetX, size_t offsetY, size_t sizeX, size_t sizeY) override;
		virtual void bind(uint32_t slot = 0) const override;

		virtual TextureHandle getRendererID() const override { return m_RendererID; };

	private:
		GLuint m_RendererID = 0;
	};

}
