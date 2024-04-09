#include "C78EPCH.h"

#ifdef C78_PLATFORM_WINDOWS

#include "C78E/Utils/PlatformUtils.h"
#include "C78E/Core/Application.h"

#include <commdlg.h>
#include <shlobj.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace C78E {

	float Time::getTime() {
		return	static_cast<float>(glfwGetTime());
	}

	C78E::FilePath FileDialogs::openFile(std::string filter, C78E::FilePath baseDir, Flags flags) {
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);

		if (baseDir.empty()) {
			if (GetCurrentDirectoryA(256, currentDir)) {}
			else C78_CORE_ERROR("FileDialogs::openFile: could not retrieve current directory!");
		}
		else {
			memcpy_s(currentDir, baseDir.string().size() + 1, baseDir.string().c_str(), baseDir.string().size() + 1);
		}
		ofn.lpstrInitialDir = currentDir;

		ofn.lpstrFilter = filter.c_str();
		ofn.nFilterIndex = 1;
		ofn.Flags = flags; // natively compatible!

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return C78E::FilePath();

	}

	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
		if (uMsg == BFFM_INITIALIZED) {
			std::string tmp = (const char*)lpData;
			C78_CORE_TRACE("WindoesPlatformUtils::BrowseCallbackProc: Path: {}", tmp);
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		}
		return 0;
	}

	FilePath FileDialogs::openFolder(C78E::FilePath baseDir, Flags flags) {
		const char* path_param = baseDir.string().c_str();

		TCHAR path[MAX_PATH];
		BROWSEINFO bi = { 0 };
		bi.lpszTitle = (const wchar_t*)"Browse for folder...";
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE; //Flags impl!
		bi.lpfn = BrowseCallbackProc;
		bi.lParam = (LPARAM)path_param;

		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
		if (pidl != 0) {
			//get the name of the folder and put it in path
			SHGetPathFromIDList(pidl, path);

			//free memory used
			IMalloc* imalloc = 0;
			if (SUCCEEDED(SHGetMalloc(&imalloc))) {
				imalloc->Free(pidl);
				imalloc->Release();
			}

			return path;
		}

		return "";
	}

	C78E::FilePath FileDialogs::saveFile(std::string filter) {
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter.c_str();
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter.c_str(), '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
		
		return C78E::FilePath();
	}

	std::string FileDialogs::constructFilter(std::vector<std::string> extensions, std::vector<std::string> labels) {
		std::string filter = "";

		for (uint32_t i = 0; i < extensions.size(); i++) {

			// TODO 

		}
		return filter;
	}

	System::Monitor System::getPrimaryMonitor() {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		C78_CORE_ASSERT(monitor, "glfw Monitor is nullptr!");
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

}

#endif