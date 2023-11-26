#pragma once

#include "C78E/Core/Core.h"
#include "C78E/Renderer/Assets/Texture/RawImage.h"

#include <string>

namespace C78E {

	

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual std::string getName() const = 0;
		virtual uint32_t getRendererID() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;
		virtual void bind(uint32_t slot = 0) const = 0;

		virtual bool isLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		struct TextureSpecification
		{
			uint32_t width = 1;
			uint32_t height = 1;
			ImageFormat format = ImageFormat::RGBA8;
			bool generateMips = true;
		};
	public:
		static Ref<Texture2D> create(const TextureSpecification& specification);
		static Ref<Texture2D> create(RawImage& image);
		
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual const TextureSpecification& getSpecification() const = 0;

	};

	class CubeMap : public Texture {
	public:
		struct TextureSpecification
		{
			uint32_t size = 1;
			ImageFormat format = ImageFormat::RGBA8;
		};
	public:
		static Ref<CubeMap> create(std::vector<RawImage>& images);
		static Ref<CubeMap> create(Ref<RawImage> crossCubeMap);

		virtual uint32_t getSize() const = 0;
		virtual const CubeMap::TextureSpecification& getSpecification() const = 0;

	};

}
