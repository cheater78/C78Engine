#pragma once

#include "C78E/Core/Core.h"
#include "C78E/Renderer/Assets/Texture/RawImage.h"

#include <string>

namespace C78E {

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual std::string getName() const = 0;

		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual const std::string& GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> create(const TextureSpecification& specification);
		static Ref<Texture2D> create(RawImage& image);
		static Ref<Texture2D> create(const std::string& path);

	};

	class CubeMap{
	public:
		static Ref<CubeMap> create(std::vector<RawImage>& images);

		virtual void bind(uint32_t slot = 0) const = 0;

	};

}
