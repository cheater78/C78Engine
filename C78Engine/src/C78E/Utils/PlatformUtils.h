#pragma once
#include "StdUtils.h"
#include <C78E/Core/FileSystem/FileSystem.h>

namespace C78E {

	class FileDialogs {
	public:
		using Filter = std::vector<FileSystem::EntryType>;
	public:
		// on fail, return empty
		static FilePath openFile(const Filter& filter, C78E::FilePath baseDirectory = "", C78E::FilePath defaultFile = "", const std::string& dialogTitle = "C78E Open File");
		static std::vector<FilePath> openFiles(const Filter& filter, C78E::FilePath baseDirectory = "", C78E::FilePath defaultFile = "", const std::string& dialogTitle = "C78E Open File(s)");
		static FilePath saveFile(const Filter& filter, C78E::FilePath baseDirectory = "", C78E::FilePath defaultFile = "", const std::string& dialogTitle = "C78E Save File");

	};

	class Time {
	public:
		static float getTime();
	};

	class System {
	public:
		struct Monitor {
			uint32_t width = 1;
			uint32_t height = 1;
			uint32_t refreshRate = 60;
			uint32_t depthR = 8;
			uint32_t depthG = 8;
			uint32_t depthB = 8;
		};
	public:
		static Monitor getPrimaryMonitor();
		static FilePath getExecutionBinaryPath();
		
	};

}
