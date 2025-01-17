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
		};

		struct Meta {
			Type type = Type::None;
			std::string name = C78E_DEFAULT_ASSET_NAME;
			FilePath fileSource = ""; // relative to an AssetDirectory
			std::string inFileIdentifier = "";  // identifier of the Asset inside the source file(one source file can easily contain multiple Assets)

			operator bool() const { return type != Type::None; }
		};

		class Group : public std::map<Ref<Asset>, Ref<Asset::Meta>> {
		public:
			bool merge(const Group& other);
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

	class AssetSerializer {
	public:
		

	};

	class AssetImporter {
	public:
		AssetImporter(const FilePath& assetDirectory);
		~AssetImporter();

		Ref<Asset::Group> import(Ref<Asset::Group> assets);

		
	private:
		struct AssetImport {
		public:
			using AssetImportFunction = std::function<Ref<Asset::Group>(const FilePath&, Ref<Asset::Meta>, AssetHandle)>;
			using AssetImportFunctionMap = std::map<Asset::Type, AssetImportFunction>;
			static const AssetImportFunctionMap assetImportFunctions;

			static AssetImportFunctionMap createAssetImportFunctionMap();
		};

		Ref<Asset::Group> importAsset(AssetHandle handle, Ref<Asset::Meta> meta);

		const FilePath m_AssetDirectory;
	};

}

namespace std {
	_EXPORT_STD _NODISCARD inline string to_string(C78E::Asset::Type assetType) {
		return C78E::Asset::Type::assetTypeToString(assetType);
	}
}