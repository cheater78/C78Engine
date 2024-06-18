#include "C78EPCH.h"
#ifdef C78_PLATFORM_WINDOWS
#include <C78E/Utils/PlatformUtils.h>

#include <C78E/Core/Application.h>

#include <commdlg.h>
#include <shlobj.h>

namespace C78E {

	float Time::getTime() {
		return	static_cast<float>(glfwGetTime());
	}

	C78E::FilePath FileDialogs::openFile(std::string filter, C78E::FilePath baseDir, Flags flags) {
		C78_ERROR("FileDialogs::openFile: not implemented! -> use GUI::FileManager!");
		C78_ASSERT(false);
		return C78E::FilePath();

	}

	C78E::FilePath FileDialogs::openFolder(C78E::FilePath baseDir, Flags flags) {
		C78_ERROR("FileDialogs::openFolder: not implemented! -> use GUI::FileManager!");
		C78_ASSERT(false);
		return "";
	}

	C78E::FilePath FileDialogs::saveFile(std::string filter) {
		C78_ERROR("FileDialogs::saveFile: not implemented! -> use GUI::FileManager!");
		C78_ASSERT(false);
		return C78E::FilePath();
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

#endif // C78_PLATFORM_WINDOWS