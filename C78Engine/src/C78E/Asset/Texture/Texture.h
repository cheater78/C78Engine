#pragma once
#include <C78E/Asset/Asset.h>
#include <C78E/Asset/Texture/Image.h>
#include <C78E/Core/Buffer.h>
#include <C78E/Utils/ImGui/TextureHandle.h>

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
			operator uint8_t() const { return m_Type; }
			operator bool() const { return m_Type; }

			static std::string textureTypeToString(Texture::Type type);
			static Texture::Type textureTypeFromString(const std::string& typeString);
		private:
			uint8_t m_Type;
		};

		class Filter {
		public:
			enum : uint8_t {
				Linear,
				Nearest,
				MipMap,
				TYPE_SIZE
			};
			Filter() : m_Filter{ 0 } {}
			Filter(uint8_t filter) : m_Filter(filter) {}
			Filter(const Filter& other) = default;
			~Filter() = default;

			operator uint8_t() const { return m_Filter; }

			static std::string filterToString(Filter type);
			static Filter filterFromString(const std::string& filterString);
		private:
			uint8_t m_Filter;
		};

		class Wrap {
		public:
			enum : uint8_t {
				Repeat,
				MirroredRepeat,
				ClampToEdge,
				ClampToBorder,
				TYPE_SIZE
			};
			Wrap() : Wrap{ 0 } {}
			Wrap(uint8_t wrap) : m_Wrap(wrap) {}
			Wrap(const Wrap& other) = default;
			~Wrap() = default;

			operator uint8_t() const { return m_Wrap; }

			static std::string wrapToString(Wrap wrap);
			static Wrap wrapFromString(const std::string& wrapString);
		private:
			uint8_t m_Wrap;
		};

		class Specification { // Info about the Texture data, format, size, channel
		public:
			Specification() = default;
			Specification(Specification&) = default;
			virtual ~Specification() = default;

			Image::ImageFormat format = Image::ImageFormat::RGBA8;
			//Image::Channel channel = Image::Channel::None; //TODO: check, multiple Textures, on the same mem using different channles?
		};

		class Option { // Info about Texture usage, "Settings / Options" - type of application
		public:
			Option() = default;
			Option(Option&) = default;
			virtual ~Option() = default;

			Filter minFilter = Filter::MipMap;
			Filter magFilter = Filter::Nearest;
			bool blenU = true;
			bool blenV = true;
			float brightness = 0.f;
			float contrast = 1.f;
		};

	public:
		virtual ~Texture() = default;

		virtual void setData(const ScopedBuffer& dataBuffer) = 0;
		virtual void bind(uint32_t slot = 0) const = 0;

		virtual TextureHandle getRendererID() const = 0;

		virtual bool operator==(const Texture& other) const final { return getRendererID() == other.getRendererID(); }

	protected:
		template<typename SpecificationType>
		SpecificationType& getSpecification() {
			static_assert(std::is_base_of_v<Texture::Specification, SpecificationType>, "Texture::getSpecification: SpecificationType must be a derived class of Texture::Specification");
			return *std::dynamic_pointer_cast<SpecificationType>(this->m_Specification);
		}

		template<typename SpecificationType>
		const SpecificationType& getSpecification() const {
			static_assert(std::is_base_of_v<Texture::Specification, SpecificationType>, "Texture::getSpecification: SpecificationType must be a derived class of Texture::Specification");
			return *std::dynamic_pointer_cast<SpecificationType>(this->m_Specification);
		}

		template<typename OptionType>
		OptionType& getOption() {
			static_assert(std::is_base_of_v<Texture::Option, OptionType>, "Texture::getOption: OptionType must be a derived class of Texture::Option");
			return *std::dynamic_pointer_cast<OptionType>(this->m_Option);
		}

		template<typename OptionType>
		const OptionType& getOption() const {
			static_assert(std::is_base_of_v<Texture::Option, OptionType>, "Texture::getOption: OptionType must be a derived class of Texture::Option");
			return *std::dynamic_pointer_cast<OptionType>(this->m_Option);
		}

	public:
		virtual Asset::Type getType() const override final { return Asset::Type::Texture; };
		static Asset::Type getClassType() { return Asset::Type::Texture; };
		virtual Texture::Type getTextureType() const { return Texture::Type::None; };
		static Texture::Type getTextureClassType() { return Texture::Type::None; }
	protected:
		Ref<Specification> m_Specification;
		Ref<Option> m_Option;
	};

	class Texture2D : public Texture {
	public:
		class Specification : public Texture::Specification {
		public:
			Specification() = default;
			Specification(Specification&) = default;
			virtual ~Specification() = default;

			uint32_t width = 1;
			uint32_t height = 1;
		};

		class Option : public Texture::Option {
		public:
			Option() = default;
			Option(Option&) = default;
			virtual ~Option() = default;

			Texture::Wrap wrapU = Texture::Wrap::Repeat;
			Texture::Wrap wrapV = Texture::Wrap::Repeat;

			glm::vec2 originOffset = { 0.f, 0.f };	// du, dv [0,1]
			glm::vec2 scale = { 1.f, 1.f };			// du, dv [0,1]
			glm::vec2 turbulence = { 0.f, 0.f };	// du, dv [0,1]
		};

	protected:
		Texture2D();
	public:
		static Ref<Texture2D> create(const Image& image, const Texture2D::Option& option = Texture2D::Option());
		static Ref<Texture2D> create(const ScopedBuffer& textureData, const Texture2D::Specification& specification, const Texture2D::Option& option = Texture2D::Option());
		static Ref<Texture2D> create(TextureHandle rendererID, const Texture2D::Specification& specification);
		
		virtual void setData(const ScopedBuffer& dataBuffer) override = 0;
		virtual void setData(const ScopedBuffer& dataBuffer, size_t offsetX, size_t offsetY, size_t sizeX, size_t sizeY) = 0;
		virtual void bind(uint32_t slot = 0) const override = 0;
		virtual void bindImage(uint32_t binding = 0) const = 0;

		const Texture2D::Specification& getSpecification() const;
		Texture2D::Option& getOption();
		const Texture2D::Option& getOption() const;
		virtual TextureHandle getRendererID() const override = 0;

		virtual uint32_t getWidth() const final;
		virtual uint32_t getHeight() const final;

		static Asset::Type getClassType() { return Asset::Type::Texture; };
		virtual Texture::Type getTextureType() const { return Texture::Type::Flat; };
		static Texture::Type getTextureClassType() { return Texture::Type::Flat; }

	protected:
		Texture2D::Specification& getSpecification();
	};

	class CubeMap : public Texture {
	public:
		class Specification : public Texture::Specification {
		public:
			Specification() = default;
			Specification(const Image& image);
			Specification(Specification&) = default;
			virtual ~Specification() = default;

			uint32_t size = 1;
		};

		class Option : public Texture::Option {
		public:
			Option() = default;
			Option(Option&) = default;
			virtual ~Option() = default;

			Texture::Wrap wrapU = Texture::Wrap::Repeat;
			Texture::Wrap wrapV = Texture::Wrap::Repeat;
			Texture::Wrap wrapW = Texture::Wrap::Repeat;

			glm::vec3 turbulence = { 0.f, 0.f, 0.f };
		};

		enum class Face : uint8_t {
			X_POS = 0, Right = 0,
			X_NEG = 1, Left = 1,
			Y_POS = 2, Top = 2,
			Y_NEG = 3, Bottom = 3,
			Z_POS = 4, Back = 4,
			Z_NEG = 5, Front = 5
		};

		class CubeImageData : public std::array<Image, 6> { //simple array to make CubeMap creation easy n consistent
		public:
			CubeImageData();
			CubeImageData(std::initializer_list<Image> init);
			CubeImageData(CubeImageData& other) = default;
			~CubeImageData() = default;

			CubeMap::Specification toSpecification() const;

			Image& topImage()		{ return operator[](Face::Top); }
			Image& bottomImage()	{ return operator[](Face::Bottom); }
			Image& frontImage()		{ return operator[](Face::Front); }
			Image& backImage()		{ return operator[](Face::Back); }
			Image& leftImage()		{ return operator[](Face::Left); }
			Image& rightImage()		{ return operator[](Face::Right); }

			const Image& topImage() const		{ return operator[](Face::Top); }
			const Image& bottomImage() const	{ return operator[](Face::Bottom); }
			const Image& frontImage() const		{ return operator[](Face::Front); }
			const Image& backImage() const		{ return operator[](Face::Back); }
			const Image& leftImage() const		{ return operator[](Face::Left); }
			const Image& rightImage() const		{ return operator[](Face::Right); }

			Image& operator[](Face face);
			const Image& operator[](Face face) const;
		private:
		};

	protected:
		CubeMap();
	public:
		static Ref<CubeMap> create(const CubeImageData& cubeMapImageData, const CubeMap::Option& option = CubeMap::Option());
		static Ref<CubeMap> create(const Image& crossCubeMap, const CubeMap::Option& option = CubeMap::Option());
		static Ref<CubeMap> create(const ScopedBuffer& textureData, const CubeMap::Specification& specification, const CubeMap::Option& option = CubeMap::Option());
		static Ref<CubeMap> create(TextureHandle rendererID, const CubeMap::Specification& specification);

		virtual void setData(const ScopedBuffer& dataBuffer) override = 0;
		virtual void setData(const ScopedBuffer& dataBuffer, Face face, size_t offsetX, size_t offsetY, size_t sizeX, size_t sizeY) = 0;
		virtual void bind(uint32_t slot = 0) const override = 0;

		const CubeMap::Specification& getSpecification() const;
		CubeMap::Option& getOption();
		const CubeMap::Option& getOption() const;
		virtual TextureHandle getRendererID() const override = 0;

		virtual uint32_t getSize() const final;

		static Asset::Type getClassType() { return Asset::Type::Texture; };
		virtual Texture::Type getTextureType() const { return Texture::Type::Sphere; };
		static Texture::Type getTextureClassType() { return Texture::Type::Sphere; }

	protected:
		CubeMap::Specification& getSpecification();
	};

}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(C78E::Texture::Type type) {
		return C78E::Texture::Type::textureTypeToString(type);
	}

}