#pragma once

#include "C78E/Renderer/Assets/Texture/Texture.h"

#include <glad.h>

namespace C78E {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& specification);
		OpenGLTexture2D(RawImage& image);
		OpenGLTexture2D(const OpenGLTexture2D&) = default;
		virtual ~OpenGLTexture2D();

		virtual std::string getName() const override { return m_Name; }

		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }

		virtual uint32_t GetWidth() const override { return m_Width;  }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual const std::string& GetPath() const override { return m_Path; }
		
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		TextureSpecification m_Specification;

		std::string m_Path;
		std::string m_Name;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};


	class OpenGLCubeMap : public CubeMap {
	public:
		OpenGLCubeMap(std::vector<RawImage>& images);
		OpenGLCubeMap(Ref<RawImage> crossCubeMap);
		~OpenGLCubeMap();

		void bind(uint32_t slot = 0) const override;

	private:
		uint32_t m_Size = 1;
		ImageFormat m_Format = ImageFormat::R8;
		uint32_t m_RendererID = 0;
	};

}
