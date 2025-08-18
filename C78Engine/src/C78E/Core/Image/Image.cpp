#include <C78EPCH.h>
#include "Image.h"

namespace C78E {

	// CPUImage

	Ref<CPUImage> CPUImage::create(ImageFormat format, size_t pixelCount, const void* data) {
		return createRef<CPUImage>(format, pixelCount, data);
	}
	Ref<CPUImage> CPUImage::copy(Ref<CPUImage> image) {
		C78E_CORE_ASSERT(image, "CPUImage::copy: Source Image is nullptr!");
		C78E_CORE_ASSERT(image->isValid(), "CPUImage::copy: Source Image empty!");
		return create(image->m_Format, image->getByteSize(), image->m_Data.data());
	}

	CPUImage::CPUImage()
		: m_Format(ImageFormat::R8), m_Data(1) {
		C78E_CORE_ASSERT(m_Data, "Failed to create Image!");
		(*m_Data.as<uint8_t>()) = 0xFF; // White Pixel
	}

	CPUImage::CPUImage(ImageFormat format, size_t pixelCount, const void* data)
		: m_Format(format), m_Data(pixelCount * format.getByteSize(), data) {
	}

	CPUImage::~CPUImage() {
	}

	void CPUImage::setData(size_t copySize, const void* srcBuffer, size_t dstBufferOffset, size_t srcBufferOffset) {
		C78E_CORE_VALIDATE(srcBuffer, return, "CPUImage::setData: Source buffer is nullptr!");
		C78E_CORE_VALIDATE(dstBufferOffset < m_Data.size(), return, "CPUImage::setData: Destination buffer offset out of Bounds!");
		C78E_CORE_VALIDATE(dstBufferOffset + copySize <= m_Data.size(), return, "CPUImage::setData: CopySize does not fit DestinationBuffer!");
		std::memcpy(m_Data.data() + dstBufferOffset, (const char*)srcBuffer + srcBufferOffset, copySize);
	}

	// Image2D
	
	Ref<Image2D> Image2D::create(ImageFormat format, ImageSize size, const void* data) {
		return createRef<Image2D>(format, size, data);
	}

	Ref<Image2D> Image2D::copy(Ref<Image2D> image) {
		C78E_CORE_ASSERT(image->isValid(), "Failed to copy Image!");
		return createRef<Image2D>(image->m_Format, image->m_Size, image->getData());
	}
	Ref<Image2D> Image2D::copyCropped(Ref<Image2D> image, ImageSize origin, ImageSize size) {
		C78E_CORE_ASSERT(origin.x + size.x <= image->m_Size.x, "Cropped Image must be in bounds!");
		C78E_CORE_ASSERT(origin.y + size.y <= image->m_Size.y, "Cropped Image must be in bounds!");
		size_t imagesize = static_cast<size_t>(size.x * size.y * image->getFormat().getByteSize());
		ScopedBuffer buffer{ imagesize };
		for(uint32_t y = 0; y < size.y; y++) {
			if (void* dst = (unsigned char*)buffer.data() + (y * size.x * image->getFormat().getByteSize())) { // should never be nullptr
				std::memcpy(dst, (unsigned char*)image->getData() + (size.x * image->getFormat().getByteSize()) + ((size.y + y) * image->m_Size.x * image->getFormat().getByteSize()), size.x * image->getFormat().getByteSize());
			}
		}
		return createRef<Image2D>(image->m_Format, size, buffer.data());
	}
	Ref<Image2D> Image2D::copyCropped(Ref<Image2D> image, float ox, float oy, float dx, float dy) {
		C78E_CORE_ASSERT(ox + dx <= 1.f, "Cropped Image must be in bounds!");
		C78E_CORE_ASSERT(oy + dy <= 1.f, "Cropped Image must be in bounds!");
		ImageSize origin = static_cast<ImageSize>(Math::vec2(image->m_Size) * Math::vec2(ox, oy));
		ImageSize size = static_cast<ImageSize>(Math::vec2(image->m_Size) * Math::vec2(dx, dy));
		return copyCropped(image, origin, size);
	}

	Image2D::Image2D()
		: CPUImage(), m_Size(1, 1) {
		(*m_Data.as<uint8_t>()) = 0xFF;
	}
	Image2D::Image2D(ImageFormat format, ImageSize size, const void* data)
		: CPUImage(format, static_cast<size_t>(size.x * size.y * format.getByteSize()), data), m_Size(size) {
		C78E_CORE_ASSERT(m_Data, "Failed to create Image!");
	}
	Image2D::~Image2D() { }

	inline ImageSize Image2D::getSize() const {
		return m_Size;
	}

}