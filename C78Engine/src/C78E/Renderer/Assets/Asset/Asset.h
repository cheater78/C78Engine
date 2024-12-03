#pragma once
#include <C78E/Core/UUID.h>

namespace C78E {

	using AssetHandle = UUID;
	
	class Asset {
	public:
		class Type {
		public:
			enum : uint8_t {
				None = 0,
				Scene,

				Mesh,
				Material,

				Shader,
				Texture2D,
				CubeMap,

				Font,
				TYPE_SIZE
			};
			Type() : m_Type{0} { }
			Type(uint8_t type) : m_Type(type) { }
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

			static Type typeFromFile(const FilePath& filePath);
			static std::set<Type> typesFromFile(const FilePath& filePath);
			static std::string assetTypeToString(Type type);
			static Type assetTypeFromString(const std::string& typeString);
		private:
			uint8_t m_Type;
		};

		struct Meta {
			Type type = Type::None;
			FilePath fileSource = "";
			std::string name = C78E_DEFAULT_ASSET_NAME;

			operator bool() const { return type != Type::None; }
		};
	public:
		AssetHandle& handle() { return m_AssetHandle; }

	public:
		virtual Type getType() const { return Type::None; };
		static Type getClassType() { return Type::None; };
	private:
		AssetHandle m_AssetHandle;

		static const struct FileMap {
			std::set<std::pair<std::string, Type>> extensionMap; // manage file extensions more centralized at some point (native supported ext)

			std::set<Type> getTypesFromExtension(const FilePath& file) const;
			Type getTypeFromExtension(const FilePath& file) const;

		} c_FileMap;
	};
}

namespace std {
	_EXPORT_STD _NODISCARD inline string to_string(C78E::Asset::Type assetType) {
		return C78E::Asset::Type::assetTypeToString(assetType);
	}
}