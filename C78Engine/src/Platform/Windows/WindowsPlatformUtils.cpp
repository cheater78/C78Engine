#include "C78EPCH.h"
#include "C78E/Utils/PlatformUtils.h"
#include "C78E/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace C78E {

	float Time::getTime() {
		return	static_cast<float>(glfwGetTime());
	}

	std::string FileDialogs::openFile(const char* filter) {
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
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();

	}

	std::string FileDialogs::saveFile(const char* filter) {
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
