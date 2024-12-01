#pragma once
#include "NativeFileExtensions.h"
#include "C78E/Core/Buffer.h"

namespace C78E {

	using FileHash = size_t;
	using FilePath = std::filesystem::path;
	using FilePathHash = size_t;

	using FileSize = uintmax_t;
	using FileTime = std::filesystem::file_time_type;

	class FileSystem { // maybe to namespace, since it's sematically all static
	public:
		static const FilePath C78RootDirectory;
		static const FilePath C78EngineDirectory;
		static const FilePath C78EditorDirectory;

	public:
		enum EntryType : uint8_t {
			Directory = 0,
			Binary,
			Project,
			Scene,
			AssetRegistry,
			Image,
			Shader,
			Model,
			Mesh,
			Material,
			Font,
			Misc
		};
		static EntryType extensionToEntryType(const FilePath& filepath);
		static std::vector<std::string> extensionsFromEntryType(EntryType type);

		static std::string stringFromEntryType(EntryType type);
		static EntryType stringToEntryType(const std::string& str);

		static const std::vector<EntryType> getAssetEntryTypes() { return {Image, Shader, Model, Mesh, Material, Font}; }
	public:
		static bool exists(const FilePath& path);
		static bool isDirectory(const C78E::FilePath& filepath);
		static bool isFile(const C78E::FilePath& filepath);

		static FilePath createDirectoryIfNotPresent(const FilePath& directoryPath);
		static FilePath createFileIfNotPresent(const FilePath& filePath);

		static FilePath getRelativePathTo(const FilePath& path, const FilePath& baseDirectory = FileSystem::C78RootDirectory);
		static FilePath normalizePath(const FilePath& path);


		// File I/O
		static std::string readFileText(const FilePath& filepath);
		static Ref<ScopedBuffer> readFileBinary(const FilePath& filepath);

		static bool writeFile(const FilePath& filepath, const char* text);
		static bool writeFile(const FilePath& filepath, const std::string& text);
		static bool writeFile(const FilePath& filepath, Ref<ScopedBuffer> binary);

		static bool removeFile(const FilePath& filepath);
		static bool removeDirectory(const FilePath& directorypath);

	private:
		static const std::map<std::string, EntryType> c_EntryTypeExtensionMap;

	};

}

namespace std {
	_EXPORT_STD _NODISCARD inline string to_string(C78E::FileSystem::EntryType entryType) {
		return C78E::FileSystem::stringFromEntryType(entryType);
	}
}