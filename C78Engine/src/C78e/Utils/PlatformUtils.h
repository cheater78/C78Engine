#pragma once

#include <string>

namespace C78E {

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string openFile(const char* filter);
		static std::string saveFile(const char* filter);
	};

	class Time
	{
	public:
		static float getTime();
	};

}
