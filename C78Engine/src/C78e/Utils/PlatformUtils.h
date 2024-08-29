#pragma once

#include <string>

namespace C78E {

	class FileDialogs {
	public:
		// on fail, return empty
		static FilePath openFile(const char* filter = "", C78E::FilePath baseDir = "", C78E::FilePath baseFile = "");
		static FilePath saveFile(const char* filter = "", C78E::FilePath baseDir = "", C78E::FilePath baseFile = "");

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
	};

}
