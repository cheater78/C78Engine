#pragma once
#include <C78E/Core/Buffer/Buffer.h>
#include <C78E/Core/Image/ImageFormat.h>

namespace C78E {
	
	class CPUImage {
	public:
		static Ref<CPUImage> create(ImageFormat format, size_t pixelCount, const void* data = nullptr);
		static Ref<CPUImage> copy(Ref<CPUImage> image);
	public:
		CPUImage(); // White Single Pixel Image
		CPUImage(ImageFormat format, size_t pixelCount, const void* data = nullptr);
		CPUImage(const CPUImage& copy) = delete; // copy should be explicitly called
		virtual ~CPUImage();

		bool isValid() const { return m_Data; }
		void* getData() const { return m_Data.as<void>(); }
		char* getDataAsBytes() const { return m_Data.as<char>(); }
		template<typename T>
		T* getDataAs() const { return m_Data.as<T>(); }
		ScopedBuffer& getDataBuffer() { return m_Data; }
		const ScopedBuffer& getDataBuffer() const { return m_Data; }

		const ImageFormat& getFormat() const { return m_Format; }
		size_t getByteSize() const { return m_Data.size(); }

		void setData(size_t copySize, const void* srcBuffer, size_t dstBufferOffset = 0, size_t srcBufferOffset = 0);

	protected:
		ScopedBuffer m_Data;
		ImageFormat m_Format;
	};

	class Image2D : public CPUImage {
	public:
		static Ref<Image2D> create(ImageFormat format, ImageSize size, const void* data = nullptr);
		static Ref<Image2D> copy(Ref<Image2D> image);
		static Ref<Image2D> copyCropped(Ref<Image2D> image, ImageSize origin, ImageSize size);
		static Ref<Image2D> copyCropped(Ref<Image2D> image, float ox, float oy, float dx, float dy);
	public:
		Image2D(); // White Single Pixel Image
		Image2D(ImageFormat format, ImageSize size, const void* data = nullptr);
		Image2D(const Image2D& copy) = delete; // copy should be explicitly called
		~Image2D();

		ImageSize getSize() const;
	private:
		ImageSize m_Size = ImageSize(1, 1);
	};

	//TODO: do we need Image3D?

}