#pragma once
#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/Assets/Texture/Image.h>

namespace C78E {

	class Texture : public Asset {
	public:
		class Type {
		public:
			enum : uint8_t {
				None = 0,
				Flat,
				Sphere,
				TYPE_SIZE
			};
			Type() : m_Type{ 0 } {}
			Type(uint8_t type) : m_Type(type) {}
			Type(const Type& other) = default;
			~Type() = default;

			bool operator==(const Type& other) const { return this->m_Type == other.m_Type; }
			bool operator==(uint8_t other) const { return this->m_Type == other; }
			bool operator!=(const Type& other) const { return this->m_Type != other.m_Type; }
			bool operator!=(uint8_t other) const { return this->m_Type != other; }
			bool operator<(const Type& other) const { return this->m_Type < other.m_Type; }
			bool operator<(uint8_t other) const { return this->m_Type < other; }
			operator uint8_t() const { return m_Type; }
			operator bool() const { return m_Type; }

			static std::string assetTypeToString(Type type);
			static Type assetTypeFromString(const std::string& typeString);
		private:
			uint8_t m_Type;
		};

		struct Option {
			Type type = Type::None;
			Image::ImageFormat format = Image::ImageFormat::RGBA8;
			Image::Channel channel = Image::Channel::None;
			bool clamp = false;
			bool blenU = true;
			bool blenV = true;
			float brightness = 0.f;
			float contrast = 1.f;
		};

		struct Meta : public Asset::Meta {
			Option option;
		};

	public:
		virtual ~Texture() = default;

		virtual uint32_t getRendererID() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;
		virtual void bind(uint32_t slot = 0) const = 0;

		virtual bool isLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	public:
		virtual Asset::Type getType() const override final { return Asset::Type::Texture; };
		static Asset::Type getClassType() { return Asset::Type::Texture; };
	private:
		Option m_Option;
	};

	class Texture2D : public Texture {
	public:
		struct Option : public Texture::Option {
			uint32_t width = 1;
			uint32_t height = 1;
			bool genMipMaps = true;
			float mipMapSharpness = 1.f;

			glm::vec3 originOffset = { 0.f, 0.f, 0.f };
			glm::vec3 scale = { 1.f, 1.f, 1.f };
			glm::vec2 turbulence = { 0.f, 0.f };
		};

		struct Meta : public Texture::Meta {
			Option option;
		};

	public:
		static Ref<Texture2D> create(const Option& specification);
		static Ref<Texture2D> create(const Option& specification, const Buffer& data);
		static Ref<Texture2D> create(const Option& specification, uint32_t rendererID);
		static Ref<Texture2D> create(const Image& image);
		
		virtual void bindImage(uint32_t binding = 0) const = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual const Option& getSpecification() const = 0;

		static Asset::Type getClassType() { return Asset::Type::Texture; };
	};

	class CubeMap : public Texture {
	public:
		struct Option : public Texture::Option {
			uint32_t size = 1;
			glm::vec3 turbulence = { 0.f, 0.f, 0.f };
		};

		struct Meta : public Texture::Meta {
			Option option;
		};
	public:
		static Ref<CubeMap> create(std::vector<Image>& images);
		static Ref<CubeMap> create(Ref<Image> crossCubeMap);

		virtual uint32_t getSize() const = 0;
		virtual const CubeMap::Option& getSpecification() const = 0;


		static Type getClassType() { return Asset::Type::Texture; };
	};

	// Scalar Maps
	// Texture Maps

	// 2D Map
	// Spherical Map

	class TextureMap { // RGBA, args: clamp/wrap, blenduv, 

	};

	class BumpMap : Texture {
		class Option : Texture::Option {
			float bumpMultiplier = 1.f;
		};
	};

	class ReflectionMap {

	};
}
