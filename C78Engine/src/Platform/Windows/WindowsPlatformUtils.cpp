#include "C78EPCH.h"
#ifdef C78E_PLATFORM_WINDOWS
#include <C78E/Utils/PlatformUtils.h>

#include <C78E/Core/Application/Application.h>

#include <commdlg.h>
#include <shlobj.h>

namespace C78E {
	/*
	* creates  a file dialog
	* returns the output as string
	* hint: can't save directories!
	*/
	std::string winFileDialog(const FileDialogs::Filter& filter, FilePath baseDirectory, FilePath defaultFile, const std::string& dialogTitle, bool save, bool multiple) {
		OPENFILENAMEA ofn;

		CHAR szFile[C78E_PATH_MAX] = { 0 };
		if(!defaultFile.empty()) {
			C78E_CORE_VALIDATE(defaultFile.string().length() < C78E_PATH_MAX, return {}, "FileDialogs::openFile: baseFile is longer than {} chars!", C78E_PATH_MAX);
			memcpy_s(szFile, defaultFile.string().length(), defaultFile.string().c_str(), defaultFile.string().length());
		}

		CHAR currentDir[C78E_PATH_MAX] = { 0 };
		if(!baseDirectory.empty()) {
			C78E_CORE_VALIDATE(baseDirectory.string().length() < C78E_PATH_MAX, return {}, "FileDialogs::openFile: baseDir is longer than {} chars!", C78E_PATH_MAX);
			memcpy_s(currentDir, baseDirectory.string().length(), baseDirectory.string().c_str(), baseDirectory.string().length());
		}

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (baseDirectory.empty() && GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;


		if(!dialogTitle.empty())
			ofn.lpstrTitle = dialogTitle.c_str();
		if(!filter.empty()) {
			std::string winFilter = "All Files\0*.*\0";
			for(FileSystem::EntryType type : filter)
				if (type == FileSystem::EntryType::Directory) {
					winFilter += "";
					break;
				}
				else {
					winFilter +=  FileSystem::stringFromEntryType(type) + "\0" + std::join(FileSystem::extensionsFromEntryType(type), ";") + "\0";
				}
			winFilter += "\0";
			ofn.lpstrFilter = winFilter.c_str();
			ofn.nFilterIndex = 1;
		}

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (multiple)
			ofn.Flags |= OFN_ALLOWMULTISELECT;

		std::string out;
		if(save && GetSaveFileNameA(&ofn) == TRUE || !save && GetOpenFileNameA(&ofn) == TRUE)
			out = { ofn.lpstrFile };

		if (out.empty())
			return ((defaultFile.is_relative()) ? baseDirectory / defaultFile : defaultFile).string();
		if (out.back() == '\n')
			out.back() = '\0';
		return out;
	}

	FilePath FileDialogs::openFile(const Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
		return winFileDialog(filter, baseDirectory, defaultFile, dialogTitle, false, false);
	}

	std::vector<FilePath> FileDialogs::openFiles(const Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
		std::string out = winFileDialog(filter, baseDirectory, defaultFile, dialogTitle, false, true);
		auto frags = std::split(out, " ");
		std::vector<FilePath> paths;
		paths.reserve(frags.size());
		for (auto& frag : frags)
			paths.emplace_back(frag);
		return paths;
	}

	FilePath FileDialogs::saveFile(const Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
		return winFileDialog(filter, baseDirectory, defaultFile, dialogTitle, true, false);
	}

	float Time::getTime() {
		return	static_cast<float>(glfwGetTime());
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