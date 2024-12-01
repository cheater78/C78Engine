#include "C78EPCH.h"
#ifdef C78E_PLATFORM_WINDOWS
#include <C78E/Utils/PlatformUtils.h>

#include <C78E/Core/Application/Application.h>

#include <commdlg.h>
#include <shlobj.h>

namespace C78E {

	float Time::getTime() {
		return	static_cast<float>(glfwGetTime());
	}

	C78E::FilePath FileDialogs::openFile(const char* filter, C78E::FilePath baseDir, C78E::FilePath baseFile) {
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		C78E_CORE_ASSERT(baseFile.string().length() < 260, "FileDialogs::openFile: baseFile is longer than 260 chars!");
		memcpy_s(szFile, baseFile.string().length(), baseFile.string().c_str(), baseFile.string().length());
		CHAR currentDir[256] = { 0 };
		C78E_CORE_ASSERT(baseDir.string().length() < 256, "FileDialogs::openFile: baseDir is longer than 256 chars!");
		memcpy_s(currentDir, baseDir.string().length(), baseDir.string().c_str(), baseDir.string().length());
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (baseDir.empty() && GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();

	}

	C78E::FilePath FileDialogs::saveFile(const char* filter, C78E::FilePath baseDir, C78E::FilePath baseFile) {
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		if (baseFile.is_relative()) baseFile = std::filesystem::absolute(baseFile);
		C78E_CORE_ASSERT(baseFile.string().length() < 260, "FileDialogs::saveFile: baseFile is longer than 260 chars!");
		memcpy_s(szFile, baseFile.string().length(), baseFile.string().c_str(), baseFile.string().length());
		CHAR currentDir[256] = { 0 };
		if (baseDir.is_relative()) baseDir = std::filesystem::absolute(baseDir);
		C78E_CORE_ASSERT(baseDir.string().length() < 256, "FileDialogs::saveFile: baseDir is longer than 256 chars!");
		memcpy_s(currentDir, baseDir.string().length(), baseDir.string().c_str(), baseDir.string().length());
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (baseDir.empty() && GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

	System::Monitor System::getPrimaryMonitor() {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		C78E_CORE_ASSERT(monitor, "glfw Monitor is nullptr!");
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		return {
			static_cast<uint32_t>(mode->width), 
			static_cast<uint32_t>(mode->height), 
			static_cast<uint32_t>(mode->refreshRate), 
			static_cast<uint32_t>(mode->redBits), 
			static_cast<uint32_t>(mode->greenBits), 
			static_cast<uint32_t>(mode->blueBits)
		};
	}

	FilePath System::getExecutionBinaryPath() {
		wchar_t path[1024] = { 0 };
		GetModuleFileNameW(NULL, path, 1024);
		return path;
	}

}

#endif // C78E_PLATFORM_WINDOWS