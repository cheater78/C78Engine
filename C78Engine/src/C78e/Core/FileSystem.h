#pragma once

#include "C78E/Core/Buffer.h"

namespace C78E {

	using FilePath = std::filesystem::path;

	class FileSystem {
	public:
		// TODO: move to FileSystem class
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}
