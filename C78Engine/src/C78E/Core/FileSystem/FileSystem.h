#pragma once

#include "C78E/Core/Buffer.h"



namespace C78E {

	using FileHash = size_t;
	using FilePath = std::filesystem::path;
	using FilePathHash = size_t;

	using FileSize = uintmax_t;
	using FileTime = std::filesystem::file_time_type;

	class FileSystem {
	public:
		enum EntryType {
			DIRECTORY,
			PROJECT,
			SCENE,
			ASSETREGISTRY,
			IMAGE,
			SHADER,
			MODEL,
			MESH,
			MATERIAL,
			FONT,
			MISC
		};
		static EntryType getEntryType(C78E::FilePath filepath);
		static std::string stringFromEntryType(EntryType type);

		static bool exists(FilePath path);

		static FilePath createDirectoryIfNotPresent(FilePath directoryPath);
		static FilePath createFileIfNotPresent(FilePath filePath);

	
	public:
		static Buffer readFileBinary(const FilePath& filepath);

	private:
		static const std::map<std::string, EntryType> c_EntryTypeExtensionMap;
	};

}
