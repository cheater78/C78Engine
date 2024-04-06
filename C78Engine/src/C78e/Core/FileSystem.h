#pragma once

#include "C78E/Core/Buffer.h"

#define C78_FILE_EXT_SCENE ".sce"
#define C78_FILE_EXT_PROJECT ".pce"

namespace C78E {

	using FilePath = std::filesystem::path;

	class FileSystem {
	public:
		// TODO: move to FileSystem class
		static Buffer readFileBinary(const FilePath& filepath);
	};

}
