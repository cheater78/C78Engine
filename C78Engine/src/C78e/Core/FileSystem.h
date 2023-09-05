#pragma once

#include "C78e/Core/Buffer.h"

namespace C78e {

	class FileSystem
	{
	public:
		// TODO: move to FileSystem class
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}
