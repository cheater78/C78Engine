#include "C78EPCH.h"
#include "FileSystem.h"

namespace C78E {

	FilePath initC78ERootDirectory() {
		return std::filesystem::current_path();
	}

	const FilePath FileSystem::C78RootDirectory = initC78ERootDirectory();
	const FilePath FileSystem::C78EngineDirectory = initC78ERootDirectory() / "C78Engine";
	const FilePath FileSystem::C78EditorDirectory = initC78ERootDirectory() / "C78Editor";

	

	FileSystem::EntryType FileSystem::extensionToEntryType(const C78E::FilePath& filepath) {
		if (isDirectory(filepath))
			return EntryType::Directory;
		auto it = c_EntryTypeExtensionMap.find(filepath.extension().string());
		if (it != c_EntryTypeExtensionMap.end())
			return it->second;
		else return EntryType::Misc;
	}

	std::vector<std::string> FileSystem::extensionsFromEntryType(EntryType type) {
		std::vector<std::string> extensions;
		if(type == EntryType::Directory)
			return extensions;
		
		for(auto& kv : c_EntryTypeExtensionMap) {
			if(kv.second == type)
				extensions.emplace_back(kv.first);
		}
		if(extensions.empty())
			extensions.emplace_back("");

		return extensions;
	}


	std::string FileSystem::stringFromEntryType(EntryType type) {
		switch (type) {
		case EntryType::Directory: return "Directory";
		case EntryType::Binary: return "Binary";
		case EntryType::Project: return "Project";
		case EntryType::Scene: return "Scene";
		case EntryType::AssetRegistry: return "AssetRegistry";
		case EntryType::Image: return "Image";
		case EntryType::Shader: return "Shader";
		case EntryType::Model: return "Model";
		case EntryType::Mesh: return "Mesh";
		case EntryType::Material: return "Material";
		case EntryType::Font: return "Font";
		case EntryType::Misc: return "File";
		default: 
			C78E_CORE_ASSERT(false, "FileSystem::stringFromEntryType: uint8_t:{} is not a valid EntryType", static_cast<uint8_t>(type));
			return {};
		}
	}

	FileSystem::EntryType FileSystem::stringToEntryType(const std::string& str) {
		if(str == "Directory") return EntryType::Directory;
		if(str == "Binary") return EntryType::Binary;
		if(str == "Project") return EntryType::Project;
		if(str == "Scene") return EntryType::Scene;
		if(str == "AssetRegistry") return EntryType::AssetRegistry;
		if(str == "Image") return EntryType::Image;
		if(str == "Shader") return EntryType::Shader;
		if(str == "Model") return EntryType::Model;
		if(str == "Mesh") return EntryType::Mesh;
		if(str == "Material") return EntryType::Material;
		if(str == "Font") return EntryType::Font;
		if(str == "Misc") return EntryType::Misc;
		C78E_CORE_ASSERT(false, "FileSystem::stringToEntryType: String {} does not represent a EntryType", str);
		return static_cast<EntryType>(-1);
	}



	bool FileSystem::exists(const FilePath& path) {
		return std::filesystem::exists(path);
	}

	bool FileSystem::isDirectory(const FilePath& filepath) {
		return !std::filesystem::is_regular_file(filepath) && !filepath.has_extension(); //TODO: maybe impl own FilePath, so dirs end in /
	}

	bool FileSystem::isFile(const FilePath& filepath) {
		return !std::filesystem::is_directory(filepath) && filepath.has_extension(); //TODO: maybe impl own FilePath, so files don't end in /
	}

	FilePath FileSystem::createDirectoryIfNotPresent(const FilePath& directoryPath) {
		C78E_CORE_ASSERT(isDirectory(directoryPath), "FileSystem::createDirectoryIfNotPresent: DirectoryPath is not a Directory!");
		if (!std::filesystem::exists(directoryPath))
			std::filesystem::create_directories(directoryPath);
		return directoryPath;
	}

	FilePath FileSystem::createFileIfNotPresent(const FilePath& filePath) {
		C78E_CORE_ASSERT(isFile(filePath), "FileSystem::createFileIfNotPresent: FilePath is not a file!");
		if (!std::filesystem::exists(filePath)) {
			if(filePath.has_parent_path()) FileSystem::createDirectoryIfNotPresent(filePath.parent_path());
			std::ofstream output(filePath); //create File
		}
		return filePath;
	}

	FilePath FileSystem::getRelativePathTo(const FilePath& path, const FilePath& baseDirectory) {
		C78E_CORE_ASSERT(isDirectory(baseDirectory), "FileSystem::getRelativePathTo: baseDirectory is not a Directory!");
		C78E_CORE_ASSERT(baseDirectory.is_absolute(), "FileSystem::getRelativePathTo: baseDirectory is not an absolute Path!");
		FilePath relPath = std::filesystem::relative(path, baseDirectory);
		return (!relPath.empty()) ? relPath : path;
	}
	
	FilePath FileSystem::normalizePath(const FilePath& path) {
		return std::filesystem::weakly_canonical(path);
	}




	std::string FileSystem::readFileText(const FilePath& filepath) {
		C78E_CORE_VALIDATE(!filepath.empty(), return {}, "FileSystem::readFileText: Provided filepath was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return {}, "FileSystem::readFileText: Provided filepath was not absolute!");
		std::ifstream stream(filepath);
		C78E_CORE_VALIDATE(stream.good(), return {}, "FileSystem::readFileText: Failed to read File: {}", filepath);
		std::ostringstream oss;
		oss << stream.rdbuf();
		return oss.str();
	}

	Ref<ScopedBuffer> FileSystem::readFileBinary(const FilePath& filepath) {
		C78E_CORE_VALIDATE(!filepath.empty(), return nullptr, "FileSystem::readFileBinary: Provided filepath was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return nullptr, "FileSystem::readFileBinary: Provided filepath was not absolute!");
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
		C78E_CORE_VALIDATE(stream, return nullptr, "FileSystem::readFileBinary: Failed to read File: {}", filepath);

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		FileSize size = end - stream.tellg();

		C78E_CORE_VALIDATE(size, return nullptr, "FileSystem::readFileBinary: Tried to read empty file: {}", filepath);
		Ref<ScopedBuffer> buffer = createRef<ScopedBuffer>(size);
		stream.read(buffer->as<char>(), size);
		stream.close();
		return buffer;
	}

	bool FileSystem::writeFile(const FilePath& filepath, const char* text) {
		C78E_CORE_VALIDATE(!filepath.empty(), return false, "FileSystem::writeFile: Provided filepath was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return false, "FileSystem::writeFile: Provided filepath was not absolute!");
		std::ofstream stream(FileSystem::createFileIfNotPresent(filepath));
		C78E_CORE_VALIDATE(stream.good(), return false, "FileSystem::writeFile: Failed to write Text File: {}", filepath);
		stream << text;
		return true;
	}

	bool FileSystem::writeFile(const FilePath& filepath, const std::string& text){
		C78E_CORE_VALIDATE(!filepath.empty(), return false, "FileSystem::writeFile: Provided filepath was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return false, "FileSystem::writeFile: Provided filepath was not absolute!");
		return writeFile(filepath, text.c_str());
	}

	bool FileSystem::writeFile(const FilePath& filepath, Ref<ScopedBuffer> binary){
		C78E_CORE_VALIDATE(!filepath.empty(), return false, "FileSystem::writeFile: Provided filepath was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return false, "FileSystem::writeFile: Provided filepath was not absolute!");
		std::ofstream stream(FileSystem::createFileIfNotPresent(filepath));
		C78E_CORE_VALIDATE(stream.good(), return false, "FileSystem::writeFile: Failed to write Binary File: {}", filepath);
		stream.write(binary->as<char>(), binary->size());
		return true;
	}
	
	bool FileSystem::removeFile(const FilePath& filepath) {
		C78E_CORE_ASSERT(isFile(filepath), "FileSystem::removeFile: FilePath is not a file!");
		return std::filesystem::remove(filepath);
	}

	bool FileSystem::removeDirectory(const FilePath& directorypath) {
		C78E_CORE_ASSERT(isDirectory(directorypath), "FileSystem::removeDirectory: DirectoryPath is not a Directory!");
		return std::filesystem::remove_all(directorypath);
	}



	const std::map<std::string, FileSystem::EntryType> FileSystem::c_EntryTypeExtensionMap = {
		{ C78E_FILE_EXT_PROJECT, EntryType::Project },
		{ C78E_FILE_EXT_SCENE, EntryType::Scene },
		{ C78E_FILE_EXT_ASSETREGISTRY, EntryType::AssetRegistry },

		{ C78E_FILE_EXT_BINARY, EntryType::Binary },

		{ ".png", EntryType::Image },
		{ ".jpg", EntryType::Image },
		{ ".jpeg", EntryType::Image },

		{ ".obj", EntryType::Model },
		{ ".mtl", EntryType::Material },

		{ ".glsl", EntryType::Shader },
		{ ".ttf", EntryType::Font },
		
		{ "", EntryType::Misc },
	};

}
