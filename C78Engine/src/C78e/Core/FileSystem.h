#pragma once

#include "C78E/Core/Buffer.h"

#define C78_FILE_EXT_SCENE ".sce"
#define C78_FILE_EXT_PROJECT ".pce"
#define C78_FILE_EXT_ASSETREGISTRY ".ace"

namespace C78E {

	using FileHash = size_t;
	using FilePath = std::filesystem::path;
	using FilePathHash = size_t;

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
	private:
		static const std::map<std::string, EntryType> c_EntryTypeExtensionMap;


	public:
		static Buffer readFileBinary(const FilePath& filepath);
	};

}
