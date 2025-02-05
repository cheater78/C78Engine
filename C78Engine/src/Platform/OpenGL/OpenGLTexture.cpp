#include "C78EPCH.h"
#include "OpenGLTexture.h"

#include <C78E/Asset/Texture/Image.h>

namespace C78E {

	//GL (conversion) Helpers

	static GLenum toGLDataFormat(Image::ImageFormat format) {
		switch ((uint8_t)format) {
		case Image::ImageFormat::R8:		return GL_RED;
		case Image::ImageFormat::R32:		return GL_RED_INTEGER;
		case Image::ImageFormat::RG8:		return GL_RG;
		case Image::ImageFormat::RGB8:		return GL_RGB;
		case Image::ImageFormat::RGBA8:		return GL_RGBA;
		case Image::ImageFormat::RGBA32F:	return GL_RGBA;
		case Image::ImageFormat::D24S8:		return GL_DEPTH_STENCIL_ATTACHMENT;
		default: C78E_CORE_ASSERT(false);	return 0;
		}
	}

	static GLenum toGLInternalFormat(Image::ImageFormat format) {
		switch ((uint8_t)format) {
		case Image::ImageFormat::R8:		return GL_R8;
		case Image::ImageFormat::R32:		return GL_R32I;
		case Image::ImageFormat::RG8:		return GL_RG8;
		case Image::ImageFormat::RGB8:		return GL_RGB8;
		case Image::ImageFormat::RGBA8:		return GL_RGBA8;
		case Image::ImageFormat::RGBA32F:	return GL_RGBA32F;
		case Image::ImageFormat::D24S8:		return GL_DEPTH24_STENCIL8;
		default: C78E_CORE_ASSERT(false);	return 0;
		}
	}

	static GLenum toGLElementFormat(Image::ImageFormat format) {
		if (format == Image::ImageFormat::RGBA32F) return GL_FLOAT;
		else return GL_UNSIGNED_BYTE;
	}

	static GLenum toGLFilter(Texture::Filter filter) {
		switch (filter) {
		case Texture::Filter::Linear: return GL_LINEAR;
		case Texture::Filter::Nearest: return GL_NEAREST;
		case Texture::Filter::MipMap: return GL_MIPMAP;
		default: C78E_CORE_ASSERT(false);	return 0;
		}
	}

	static GLenum toGLWrap(Texture::Wrap wrap) {
		switch (wrap) {
		case Texture::Wrap::Repeat: return GL_REPEAT;
		case Texture::Wrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
		case Texture::Wrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
		case Texture::Wrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
		default: C78E_CORE_ASSERT(false);	return 0;
		}
	}


	// OpenGLTexture2D

	static void createGLTexture2D(GLuint& rendererID, const Texture2D::Specification& specification, const Texture2D::Option& option) {
		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, toGLInternalFormat(specification.format), specification.width, specification.height);

		if (option.minFilter == Texture::Filter::MipMap)
			glGenerateTextureMipmap(rendererID);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, toGLFilter(option.minFilter));
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, toGLFilter(option.magFilter));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, toGLWrap(option.wrapU));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, toGLWrap(option.wrapV));
	}

	static void writeGLTexture2D(GLuint& rendererID, const ScopedBuffer& textureData, const Texture2D::Specification& specification, size_t offsetX = 0, size_t offsetY = 0, size_t bufSizeX = 0, size_t bufSizeY = 0) {
		const size_t textureByteSize = specification.width * specification.height * Image::ImageFormat::imageFormatSize(specification.format);
		C78E_CORE_ASSERT(textureData.size() <= textureByteSize, "OpenGLTexture2D::writeGLTexture2D: Data did not fit in the Texture memory!");
		glTextureSubImage2D(
			rendererID,										// target
			0,												// mip map level
			static_cast<GLsizei>(offsetX), static_cast<GLsizei>(offsetY),								// offset
			(bufSizeX) ? static_cast<GLsizei>(bufSizeX) : static_cast<GLsizei>(specification.width),	// size x
			(bufSizeY) ? static_cast<GLsizei>(bufSizeY) : static_cast<GLsizei>(specification.height),	// size y
			toGLDataFormat(specification.format),			// color format
			toGLElementFormat(specification.format),		// color element format
			textureData.as<void>()							// data array
		);
	}


	OpenGLTexture2D::OpenGLTexture2D(const Image& image, const Texture2D::Option& option) {
		Texture2D::Specification& spec = getSpecification();
		spec.width = image.getWidth();
		spec.height = image.getHeight();
		spec.format = image.getFormat();

		Texture2D::Option& opt = getOption();
		opt = option;

		createGLTexture2D(m_RendererID, spec, opt);
		writeGLTexture2D(m_RendererID, image.getDataBuffer(), spec);
	}

	OpenGLTexture2D::OpenGLTexture2D(const ScopedBuffer& textureData, const Texture2D::Specification& specification, const Texture2D::Option& option) {
		Texture2D::Specification& spec = getSpecification();
		spec = specification;

		Texture2D::Option& opt = getOption();
		opt = option;

		createGLTexture2D(m_RendererID, spec, opt);
		writeGLTexture2D(m_RendererID, textureData, spec);
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureHandle rendererID, const Texture2D::Specification& specification)
		: m_RendererID(glIsTexture(rendererID) ? (uint32_t)rendererID : 0) {
		Texture2D::Specification& spec = getSpecification();
		spec = specification;
		C78E_CORE_ERROR("OpenGLTexture2D::OpenGLTexture2D: creation using TextureHandle is not safe!");
		//spec and opt unknown...?
		//dont free, validate before every use...
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		if (m_RendererID && glIsTexture(m_RendererID))
			glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::setData(const ScopedBuffer& dataBuffer) {
		writeGLTexture2D(m_RendererID, dataBuffer, getSpecification());
	}

	void OpenGLTexture2D::setData(const ScopedBuffer& dataBuffer, size_t offsetX, size_t offsetY, size_t sizeX, size_t sizeY) {
		writeGLTexture2D(m_RendererID, dataBuffer, getSpecification(), offsetX, offsetY, sizeX, sizeY);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::bindImage(uint32_t binding) const {
		glBindImageTexture(binding, m_RendererID, 0, GL_TRUE, 0, GL_READ_WRITE, toGLInternalFormat(getSpecification().format));
	}


	// OpenGLCubeMap

	static void createGLCubeMap(GLuint& rendererID, const CubeMap::Specification& specification, const CubeMap::Option& option) {
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, toGLInternalFormat(specification.format), specification.size, specification.size);

		if (option.minFilter == Texture::Filter::MipMap)
			glGenerateTextureMipmap(rendererID);
		
		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, toGLFilter(option.minFilter));
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, toGLFilter(option.magFilter));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, toGLWrap(option.wrapU));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, toGLWrap(option.wrapV));
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_R, toGLWrap(option.wrapW));
	}
	
	static void writeGLCubeMap(GLuint& rendererID, CubeMap::Face face, const ScopedBuffer& textureData, const CubeMap::Specification& specification, size_t offsetX = 0, size_t offsetY = 0, size_t sizeX = 0, size_t sizeY = 0) {
		glTextureSubImage3D(
			rendererID,																				// target
			0,																						// mip map level
			static_cast<GLsizei>(offsetX), static_cast<GLsizei>(offsetY), (uint8_t)face,														// offset, z = side
			(sizeX) ? static_cast<GLsizei>(sizeX) : static_cast<GLsizei>(specification.size),		// size x
			(sizeY) ? static_cast<GLsizei>(sizeY) : static_cast<GLsizei>(specification.size),		// size y
			1,																						// size z(const.)
			toGLDataFormat(specification.format),													// color format
			toGLElementFormat(specification.format),												// color element format
			textureData.as<void>()																	// data array
		);
	}

	OpenGLCubeMap::OpenGLCubeMap(const CubeImageData& cubeMapImageData, const CubeMap::Option& option) {
		CubeMap::Specification& spec = getSpecification();
		spec = cubeMapImageData.toSpecification();

		CubeMap::Option& opt = getOption();
		opt = option;

		for(Face f = (Face)0; (uint8_t)f < 6; ((uint8_t&)f)++)
			writeGLCubeMap(m_RendererID, f, cubeMapImageData[f].getDataBuffer(), spec);
	}

	OpenGLCubeMap::OpenGLCubeMap(const Image& crossCubeMap, const CubeMap::Option& option) {
		CubeMap::Specification& spec = getSpecification();
		spec.size = std::min<uint32_t>(crossCubeMap.getWidth() / 4, crossCubeMap.getHeight() / 3);
		spec.format = crossCubeMap.getFormat();

		CubeMap::Option& opt = getOption();
		opt = option;

		const size_t size = spec.size;
		CubeImageData cubeData{
			crossCubeMap.croppedCopy(2 * size, 1 * size, size, size),
			crossCubeMap.croppedCopy(0 * size, 1 * size, size, size),
			crossCubeMap.croppedCopy(1 * size, 0 * size, size, size),
			crossCubeMap.croppedCopy(1 * size, 2 * size, size, size),
			crossCubeMap.croppedCopy(1 * size, 1 * size, size, size),
			crossCubeMap.croppedCopy(3 * size, 1 * size, size, size),
		};
		
		for (Face f = (Face)0; (uint8_t)f < 6; ((uint8_t&)f)++)
			writeGLCubeMap(m_RendererID, f, cubeData[f].getDataBuffer(), spec);
	}

	OpenGLCubeMap::OpenGLCubeMap(const ScopedBuffer& textureData, const CubeMap::Specification& specification, const CubeMap::Option& option) {
		CubeMap::Specification& spec = getSpecification();
		spec = specification;

		CubeMap::Option& opt = getOption();
		opt = option;

		setData(textureData);
	}

	OpenGLCubeMap::OpenGLCubeMap(TextureHandle rendererID, const CubeMap::Specification& specification)
		: m_RendererID(glIsTexture(rendererID) ? (uint32_t)rendererID : 0) {
		CubeMap::Specification& spec = getSpecification();
		spec = specification;
		C78E_CORE_ERROR("OpenGLCubeMap::OpenGLCubeMap: creation using TextureHandle is not safe!");
		//spec and opt unknown...?
		//dont free, validate before every use...
	}

	OpenGLCubeMap::~OpenGLCubeMap() {
		if (m_RendererID && glIsTexture(m_RendererID))
			glDeleteTextures(1, &m_RendererID);
	}
	
	/*
	* dataBuffer must contain the full texture data, sequential order as specified in CubeMap::Face, 
	*/
	void OpenGLCubeMap::setData(const ScopedBuffer& dataBuffer) {
		CubeMap::Specification& spec = getSpecification();
		C78E_CORE_ASSERT(dataBuffer.size() == spec.size * spec.size * Image::ImageFormat::imageFormatSize(spec.format), "OpenGLCubeMap::setData: BufferSize doesn't match the Texture Memory Size!");

		const size_t perFaceBufferSize = dataBuffer.size() / 6;
		const size_t faceSideByteSize = static_cast<size_t>(std::sqrt(perFaceBufferSize));
		const size_t faceSideLength = faceSideByteSize / Image::ImageFormat::imageFormatSize(spec.format);

		C78E_CORE_ASSERT(spec.size == faceSideLength, "OpenGLCubeMap::setData: BufferSize doesn't match the Texture Specs size!"); //prob redundant

		for (Face f = (Face)0; (uint8_t)f < 6; ((uint8_t&)f)++) {
			ScopedBuffer faceBuffer{ perFaceBufferSize };
			const void* faceDataBasePtr = (void*)((size_t)dataBuffer.as<void>() + ((size_t)f * perFaceBufferSize));
			std::memcpy(faceBuffer.as<void>(), faceDataBasePtr, perFaceBufferSize);

			writeGLCubeMap(m_RendererID, f, faceBuffer, spec);
		}
	}

	void OpenGLCubeMap::setData(const ScopedBuffer& dataBuffer, Face face, size_t offsetX, size_t offsetY, size_t sizeX, size_t sizeY) {
		writeGLCubeMap(m_RendererID, face, dataBuffer, getSpecification(), offsetX, offsetY, sizeX, sizeY);
	}

	void OpenGLCubeMap::bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}
	
}
