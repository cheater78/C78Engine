#pragma once

#include <string>

namespace C78E {

	class FileDialogs {
	public:
		typedef uint32_t Flags;
		enum Flag : uint32_t { // don't change -> compatible with win::GetOpenFileNameA
			None = 0,

			ReadOnly = 1<<0,
			HideReadOnly = 1<<2,
			NoReadOnlyReturn = 1<<15,

			NoChangeDirectory = 1<<3,

			AllowMultiSelect = 1<<9,

			ExtensionDifferent = 1<<10,

			PathMustExist = 1<<11,
			FileMustExist = 1<<12
		};
	public:
		// on fail, return empty
		static FilePath openFile(std::string filter = "", C78E::FilePath baseDir = "", Flags flags = FileMustExist | PathMustExist);
		static FilePath openFolder(C78E::FilePath baseDir = "", Flags flags = PathMustExist);
		static FilePath saveFile(std::string filter = "");
		static std::string constructFilter(std::vector<std::string> extensions, std::vector<std::string> labels = {});

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
