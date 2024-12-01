#include "C78EPCH.h"
#ifdef C78E_PLATFORM_LINUX

namespace C78E {

	/*
	* Executes a shell cmd and returns the stdout(limited to 128 chars)
	*/
	std::string exec(const char* cmd) {
		std::array<char, 128> buffer;
		std::string result;
		std::unique_ptr<FILE, void(*)(FILE*)> pipe(popen(cmd, "r"),
			[](FILE * f) -> void {
				std::ignore = pclose(f);
			});
		C78E_CORE_ASSERT(pipe, "LinuxPlatformUtils//exec: Failed to create pipe!");
		while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
			result += buffer.data();
		}
		return result;
	}

	/*
	* creates the linux shell cmd to create a zenity based file dialog
	* hint: can't save directories!
	*/
	std::string zenityFileDialogCall(const FileDialogs::Filter& filter = {}, const std::string& title = "", const FilePath& filepath = "", bool save = false, bool blocking = true, bool multiple = false) {
		const FilePath zenity = "/usr/bin/zenity";
		C78E_CORE_ASSERT(FileSystem::exists(zenity), "FileDialogs::openFile: zenity not found!, Linux File Dialogs require /usr/bin/zenity");
		std::string call = zenity.string() + " --file-selection ";
		if(blocking)
			call += " --modal";
		if(!filepath.empty())
			call += " --filename=\"" + filepath.string() + "\"";
		if(!title.empty())
			call += " --title=\"" + title + "\"";
		if(!filter.empty())
			for(FileSystem::EntryType type : filter)
				if(type == FileSystem::EntryType::Directory){
					call += " --directory";
					break;
				} else {
					call += " --file-filter=\""
						+ FileSystem::stringFromEntryType(type)
						+ " | " + std::join(FileSystem::extensionsFromEntryType(type), " ") + "\"";
				}
		if(save)
			call += " --save";

		return call;
	}

	/*
	* creates  a zenity based file dialog
	* returns the output as string
	* hint: can't save directories!
	*/
	std::string zenityFileDialog(const FileDialogs::Filter& filter, FilePath baseDirectory, FilePath defaultFile, const std::string& dialogTitle, bool save, bool multiple) {
		const std::string call = zenityFileDialogCall(filter, dialogTitle, baseDirectory, save, false, multiple);
		std::string result = exec(call.c_str());
		if(result.empty())
			return (defaultFile.is_relative()) ? baseDirectory / defaultFile : defaultFile;
		if(result.back() == '\n')
			result.back() = '\0';
		return result;
	}

	C78E::FilePath FileDialogs::openFile(const FileDialogs::Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
		return FilePath(zenityFileDialog(filter, baseDirectory, defaultFile, dialogTitle, false, false));
	}

	std::vector<FilePath> FileDialogs::openFiles(const FileDialogs::Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
		const std::vector<std::string> zenityResponse = std::split(zenityFileDialog(filter, baseDirectory, defaultFile, dialogTitle, false, true), '|');
		std::vector<FilePath> filePaths;
		filePaths.reserve(zenityResponse.size());
		for(std::string str : zenityResponse)
			filePaths.emplace_back(str);
		return filePaths;
	}

	C78E::FilePath FileDialogs::saveFile(const FileDialogs::Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
		return FilePath(zenityFileDialog(filter, baseDirectory, defaultFile, dialogTitle, true, false));
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

	FilePath System::getExecutionBinaryPath(){
		char result[1024];
		ssize_t count = readlink("/proc/self/exe", result, 1024);
		return std::string(result, (count > 0) ? count : 0);
	}

}

#endif // C78E_PLATFORM_LINUX