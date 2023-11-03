#include "C78EPCH.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "C78E/Renderer/Assets/Texture/RawImage.h"

namespace C78E {

	namespace Utils {

		static GLenum C78EImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::R8:		return GL_RED;
				case ImageFormat::RG8:		return GL_RG;
				case ImageFormat::RGB8:		return GL_RGB;
				case ImageFormat::RGBA8:	return GL_RGBA;
				case ImageFormat::RGBA32F:	return GL_RGBA;
			}

			C78_CORE_ASSERT(false);
			return 0;
		}
		
		static GLenum C78EImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::R8:		return GL_R8;
				case ImageFormat::RG8:		return GL_RG8;
				case ImageFormat::RGB8:		return GL_RGB8;
				case ImageFormat::RGBA8:	return GL_RGBA8;
				case ImageFormat::RGBA32F:	return GL_RGBA32F;
			}

			C78_CORE_ASSERT(false);
			return 0;
		}

	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		m_InternalFormat = Utils::C78EImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::C78EImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		m_Name = "<unknown>";
	}

	OpenGLTexture2D::OpenGLTexture2D(RawImage& image)
	{	
		if (image.isValid())
		{
			m_IsLoaded = true;

			m_Width = image.getWidth();
			m_Height = image.getHeight();
			m_InternalFormat = Utils::C78EImageFormatToGLInternalFormat(image.getFormat());
			m_DataFormat = Utils::C78EImageFormatToGLDataFormat(image.getFormat());

			C78_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, image.getData());
		}
		m_Name = std::filesystem::getName(image.getName());
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		//C78_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		//C78_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		C78_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		//C78_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}













	OpenGLCubeMap::OpenGLCubeMap(std::vector<RawImage>& images) {
		C78_CORE_ASSERT(images.size() == 6, "CubeMap data vector must have exactly 6 imgs!");
		C78_CORE_ASSERT(images[0].getHeight() == images[0].getWidth(), "CubeMap Textures must be sqares!");

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		uint32_t side = 0;
		m_Size = images[0].getWidth();
		m_Format = images[0].getFormat();
		for (auto& image : images) {
			C78_CORE_ASSERT(image.getWidth() == m_Size && image.getHeight() == m_Size && image.getFormat() == m_Format, "CubeMap Textures must be all the same size!");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, Utils::C78EImageFormatToGLInternalFormat(m_Format), m_Size, m_Size, 0, Utils::C78EImageFormatToGLDataFormat(m_Format), GL_UNSIGNED_BYTE, image.getData());
			side++;
		}
	}

	OpenGLCubeMap::OpenGLCubeMap(Ref<RawImage> crossCubeMap)
	{
		C78_CORE_ASSERT(crossCubeMap, "CubeMap data cannot be null!");
		C78_CORE_ASSERT((uint32_t)crossCubeMap->getFormat() < 5, "Img Format not supported!");

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		m_Format = crossCubeMap->getFormat();

		auto map = *crossCubeMap;
		m_Size = (map.getWidth() / 4 > map.getHeight() / 3) ? (map.getHeight() / 3) : (map.getWidth() / 4);
		

		auto right	= map.croppedCopy(2*m_Size, 1*m_Size, m_Size, m_Size);
		auto left	= map.croppedCopy(0*m_Size, 1*m_Size, m_Size, m_Size);
		auto top	= map.croppedCopy(1*m_Size, 0*m_Size, m_Size, m_Size);
		auto bot	= map.croppedCopy(1*m_Size, 2*m_Size, m_Size, m_Size);
		auto front	= map.croppedCopy(1*m_Size, 1*m_Size, m_Size, m_Size);
		auto back	= map.croppedCopy(3*m_Size, 1*m_Size, m_Size, m_Size);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, Utils::C78EImageFormatToGLInternalFormat(m_Format), m_Size, m_Size, 0, Utils::C78EImageFormatToGLDataFormat(m_Format), GL_UNSIGNED_BYTE, right->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, Utils::C78EImageFormatToGLInternalFormat(m_Format), m_Size, m_Size, 0, Utils::C78EImageFormatToGLDataFormat(m_Format), GL_UNSIGNED_BYTE, left->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, Utils::C78EImageFormatToGLInternalFormat(m_Format), m_Size, m_Size, 0, Utils::C78EImageFormatToGLDataFormat(m_Format), GL_UNSIGNED_BYTE, top->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, Utils::C78EImageFormatToGLInternalFormat(m_Format), m_Size, m_Size, 0, Utils::C78EImageFormatToGLDataFormat(m_Format), GL_UNSIGNED_BYTE, bot->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, Utils::C78EImageFormatToGLInternalFormat(m_Format), m_Size, m_Size, 0, Utils::C78EImageFormatToGLDataFormat(m_Format), GL_UNSIGNED_BYTE, front->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, Utils::C78EImageFormatToGLInternalFormat(m_Format), m_Size, m_Size, 0, Utils::C78EImageFormatToGLDataFormat(m_Format), GL_UNSIGNED_BYTE, back->getData());

		
	}

	OpenGLCubeMap::~OpenGLCubeMap() {
		if (m_RendererID)
			glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLCubeMap::bind(uint32_t slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}
}
