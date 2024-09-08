#pragma once
#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/Assets/Texture/Image.h>

namespace C78E {

	class Texture : public Asset {
	public:
		enum TextureType {
			FLAT,
			SPHERE,
			CUBE_TOP,
			CUBE_BOTTOM,
			CUBE_FRONT,
			CUBE_BACK,
			CUBE_LEFT,
			CUBE_RIGHT
		};

		struct TextureOption {
			TextureType type = TextureType::FLAT;
			float sharpness = 1.f;
			float brightness = 0.f;
			float contrast = 1.f;
			glm::vec3 originOffset = { 0.f, 0.f, 0.f };
			glm::vec3 scale = { 1.f, 1.f, 1.f };
			glm::vec3 turbulence = { 0.f, 0.f, 0.f };
			uint32_t textureResolution = UINT32_MAX;
			bool clamp = false;
			char imfchan = 'l';
			bool blenU = true;
			bool blenV = true;
			float bumpMultiplier = 1.f;

			//ext
			std::string colorspace = "";
		};

	public:
		virtual ~Texture() = default;

		virtual uint32_t getRendererID() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;
		virtual void bind(uint32_t slot = 0) const = 0;

		virtual bool isLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	public:
		// TODO: revisit, Texture loading/handling, 2D and Cubemap don't differ as file rn
		virtual AssetType getType() const override { return Asset::AssetType::None; };

		static AssetType getClassType() { return AssetType::None; };
	public:
		// virtual
	private:
		TextureOption m_Option; //TODO: all u need to know abt this texture -> process on creation
	};

	class Texture2D : public Texture {
	public:
		struct TextureSpecification {
			uint32_t width = 1;
			uint32_t height = 1;
			Image::ImageFormat format = Image::ImageFormat::RGBA8;
			bool generateMips = true;
		};
	public:
		static Ref<Texture2D> create(const TextureSpecification& specification);
		static Ref<Texture2D> create(const TextureSpecification& specification, const Buffer& data);
		static Ref<Texture2D> create(const TextureSpecification& specification, uint32_t rendererID);
		static Ref<Texture2D> create(const Image& image);
		
		virtual void bindImage(uint32_t binding = 0) const = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual const TextureSpecification& getSpecification() const = 0;

		virtual AssetType getType() const override { return Asset::AssetType::Texture2D; };

		static AssetType getClassType() { return AssetType::Texture2D; };
	};

	class CubeMap : public Texture {
	public:
		struct TextureSpecification {
			uint32_t size = 1;
			Image::ImageFormat format = Image::ImageFormat::RGBA8;
		};
	public:
		static Ref<CubeMap> create(std::vector<Image>& images);
		static Ref<CubeMap> create(Ref<Image> crossCubeMap);

		virtual uint32_t getSize() const = 0;
		virtual const CubeMap::TextureSpecification& getSpecification() const = 0;

		virtual AssetType getType() const override { return Asset::AssetType::None; };

		static AssetType getClassType() { return AssetType::None; }; //TODO
	};

}
