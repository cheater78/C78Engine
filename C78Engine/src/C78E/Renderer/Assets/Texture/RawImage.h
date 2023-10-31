#pragma once
#include "C78ePCH.h"


namespace C78E {

	enum class ImageFormat : uint32_t {
		None	= 0,
		R8		= 1,
		RG8		= 2,
		RGB8	= 3,
		RGBA8	= 4,
		RGBA32F = 5
	};

	class RawImage {
	public:
		RawImage(const char* file, bool flipvert = false);
		RawImage(const RawImage& copy);
		~RawImage();

		bool isValid() { return m_Data; }

		std::string getName() { return m_Name; }

		uint32_t getWidth() { return m_Width; }
		uint32_t getHeight() { return m_Height; }
		ImageFormat& getFormat() { return m_Format; }
		void* getData() { return m_Data; }

		size_t getByteSize();

		void expandLeft(RawImage& image);
		void expandRight(RawImage& image);
		void expandTop(RawImage& image);
		void expandBot(RawImage& image);

	private:
		std::string m_Name = "<RawImage>";

		uint32_t m_Width = 1;
		uint32_t m_Height = 1;
		ImageFormat m_Format = ImageFormat::R8;

		void* m_Data = nullptr;

	};
}