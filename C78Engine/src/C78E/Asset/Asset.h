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

				Model,
				Mesh,
				Material,

				Shader,
				Texture,

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

			static const struct FileMap {
				std::set<std::pair<std::string, Type>> extensionMap; // manage file extensions more centralized at some point (native supported ext)

				std::set<Type> getTypesFromExtension(const FilePath& file) const;
				Type getTypeFromExtension(const FilePath& file) const;

			} c_FileMap;
		};
	public:
		virtual ~Asset() = default;

		AssetHandle& handle() { return m_AssetHandle; }
		const AssetHandle& handle() const { return m_AssetHandle; }

	public:
		virtual Type getType() const { return Type::None; };
		static Type getClassType() { return Type::None; };
	private:
		AssetHandle m_AssetHandle;
	};

}

namespace std {
	_EXPORT_STD _NODISCARD inline string to_string(C78E::Asset::Type assetType) {
		return C78E::Asset::Type::assetTypeToString(assetType);
	}

	_EXPORT_STD _NODISCARD inline string to_string(C78E::Asset asset) {
		return std::to_string(asset.handle());
	}

	template <>
	struct hash<C78E::Asset> {
		size_t operator()(const C78E::Asset& asset) const {
			return std::hash<C78E::UUID>()(asset.handle());
		}
	};

	template <>
	struct hash<C78E::Scope<C78E::Asset>> {
		size_t operator()(const C78E::Scope<C78E::Asset>& asset) const {
			return (asset) ? std::hash<C78E::Asset>()(*asset) : 0;
		}
	};

	template <>
	struct hash<C78E::Ref<C78E::Asset>> {
		size_t operator()(const C78E::Ref<C78E::Asset>& asset) const {
			return (asset) ? std::hash<C78E::Asset>()(*asset) : 0;
		}
	};
}