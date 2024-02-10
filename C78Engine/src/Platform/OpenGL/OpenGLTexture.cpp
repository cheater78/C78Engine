#include "C78EPCH.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "C78E/Assets/Texture/RawImage.h"

namespace C78E {

	OpenGLTexture2D::OpenGLTexture2D(const Texture2D::TextureSpecification& specification)
		: m_RendererID(0), m_Specification(specification), m_Name("<unknown>"), m_IsLoaded(false)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, toGLInternalFormat(m_Specification.format), m_Specification.width, m_Specification.height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const Texture2D::TextureSpecification& specification, uint32_t rendererID)
		: m_RendererID(rendererID), m_Specification(specification), m_Name("<unknown>"), m_IsLoaded(false)
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTextureStorage2D(m_RendererID, 1, toGLInternalFormat(m_Specification.format), m_Specification.width, m_Specification.height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(std::string filename) {
		RawImage image{filename.c_str(), false};
		C78_CORE_ASSERT(image.isValid(), "Image must be valid!");
		m_Name = std::filesystem::getName(image.getName());
		m_Specification.width = image.getWidth();
		m_Specification.height = image.getHeight();
		m_Specification.format = image.getFormat();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, toGLInternalFormat(m_Specification.format), m_Specification.width, m_Specification.height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLenum type;
		if (m_Specification.format == ImageFormat::RGBA32F)
			type = GL_FLOAT;
		else
			type = GL_UNSIGNED_BYTE;

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specification.width, m_Specification.height, toGLDataFormat(m_Specification.format), type, image.getData());
		
		if (m_Specification.format == ImageFormat::RGBA32F)
			

		m_IsLoaded = true;
	}

	OpenGLTexture2D::OpenGLTexture2D(RawImage& image) {
		C78_CORE_ASSERT(image.isValid(), "Image must be valid!");
		m_Name = std::filesystem::getName(image.getName());
		m_Specification.width = image.getWidth();
		m_Specification.height = image.getHeight();
		m_Specification.format = image.getFormat();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, toGLInternalFormat(m_Specification.format), m_Specification.width, m_Specification.height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLenum type;
		if (m_Specification.format == ImageFormat::RGBA32F)
			type = GL_FLOAT;
		else
			type = GL_UNSIGNED_BYTE;

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specification.width, m_Specification.height, toGLDataFormat(m_Specification.format), type, image.getData());

		if (m_Specification.format == ImageFormat::RGBA32F)


			m_IsLoaded = true;
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::setData(void* data, uint32_t size) {
		C78_CORE_ASSERT(size == m_Specification.width * m_Specification.height * imageFormatSize(m_Specification.format), "Data fit the Texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specification.width, m_Specification.height, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, data);
		m_IsLoaded = true;
	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::bindImage(uint32_t binding) const {
		glBindImageTexture(binding, m_RendererID, 0, GL_TRUE, 0, GL_READ_WRITE, toGLInternalFormat(m_Specification.format));
	}













	OpenGLCubeMap::OpenGLCubeMap(CubeMap::TextureSpecification& specification)
		: m_Name("<unknown>"), m_IsLoaded(false), m_Specification(specification)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	OpenGLCubeMap::OpenGLCubeMap(std::vector<RawImage>& images)
		: m_Name("<unknownqm>"), m_IsLoaded(true) 
	{
		C78_CORE_ASSERT(images.size() == 6, "CubeMap data vector must have exactly 6 imgs!");
		C78_CORE_ASSERT(images[0].getHeight() == images[0].getWidth(), "CubeMap Textures must be sqares!");
		m_Specification.size = images[0].getWidth();
		m_Specification.format = images[0].getFormat();

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		uint32_t side = 0;
		for (auto& image : images) {
			C78_CORE_ASSERT(image.getWidth() == m_Specification.size && image.getHeight() == m_Specification.size && image.getFormat() == m_Specification.format, "CubeMap Textures must be all the same size!");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side++, 0, toGLInternalFormat(m_Specification.format), m_Specification.size, m_Specification.size, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, image.getData());
		}
	}

	OpenGLCubeMap::OpenGLCubeMap(Ref<RawImage> crossCubeMap)
		: m_Name(crossCubeMap->getName()), m_IsLoaded(true)
	{
		C78_CORE_ASSERT(crossCubeMap, "CubeMap data cannot be null!");
		C78_CORE_ASSERT((uint32_t)crossCubeMap->getFormat() < 5, "Img Format not supported!");
		m_Specification.size = std::min<uint32_t>(crossCubeMap->getWidth() / 4, crossCubeMap->getHeight() / 3);
		m_Specification.format = crossCubeMap->getFormat();

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		uint32_t size = m_Specification.size;
		/*
		auto right	= (*crossCubeMap).croppedCopy(2*size, 1*size, size, size);
		auto left	= (*crossCubeMap).croppedCopy(0*size, 1*size, size, size);
		auto top	= (*crossCubeMap).croppedCopy(1*size, 0*size, size, size);
		auto bot	= (*crossCubeMap).croppedCopy(1*size, 2*size, size, size);
		auto front	= (*crossCubeMap).croppedCopy(1*size, 1*size, size, size);
		auto back	= (*crossCubeMap).croppedCopy(3*size, 1*size, size, size);
		*/
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, toGLInternalFormat(m_Specification.format), size, size, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, (*crossCubeMap).croppedCopy(2*size, 1*size, size, size)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, toGLInternalFormat(m_Specification.format), size, size, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, (*crossCubeMap).croppedCopy(0*size, 1*size, size, size)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, toGLInternalFormat(m_Specification.format), size, size, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, (*crossCubeMap).croppedCopy(1*size, 0*size, size, size)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, toGLInternalFormat(m_Specification.format), size, size, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, (*crossCubeMap).croppedCopy(1*size, 2*size, size, size)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, toGLInternalFormat(m_Specification.format), size, size, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, (*crossCubeMap).croppedCopy(1*size, 1*size, size, size)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, toGLInternalFormat(m_Specification.format), size, size, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, (*crossCubeMap).croppedCopy(3*size, 1*size, size, size)->getData());
	}

	OpenGLCubeMap::~OpenGLCubeMap() {
		if (m_RendererID)
			glDeleteTextures(1, &m_RendererID);
	}
	
	void OpenGLCubeMap::setData(void* data, uint32_t size) {
		C78_CORE_ASSERT(m_Specification.size * m_Specification.size * imageFormatSize(m_Specification.format) * 12 != size, "Data must fit CubeMap!"); //Maybe Broky
		uint32_t dim = m_Specification.size;
		RawImage crossCubeMap(dim, dim, m_Specification.format, data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, toGLInternalFormat(m_Specification.format), dim, dim, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, crossCubeMap.croppedCopy(2 * dim, 1 * dim, dim, dim)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, toGLInternalFormat(m_Specification.format), dim, dim, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, crossCubeMap.croppedCopy(0 * dim, 1 * dim, dim, dim)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, toGLInternalFormat(m_Specification.format), dim, dim, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, crossCubeMap.croppedCopy(1 * dim, 0 * dim, dim, dim)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, toGLInternalFormat(m_Specification.format), dim, dim, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, crossCubeMap.croppedCopy(1 * dim, 2 * dim, dim, dim)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, toGLInternalFormat(m_Specification.format), dim, dim, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, crossCubeMap.croppedCopy(1 * dim, 1 * dim, dim, dim)->getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, toGLInternalFormat(m_Specification.format), dim, dim, 0, toGLDataFormat(m_Specification.format), GL_UNSIGNED_BYTE, crossCubeMap.croppedCopy(3 * dim, 1 * dim, dim, dim)->getData());
	}

	void OpenGLCubeMap::bind(uint32_t slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}
	
}
