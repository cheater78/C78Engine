#include "C78EPCH.h"
#include "FileSystem.h"

namespace C78E {

	const std::map<std::string, FileSystem::EntryType> FileSystem::c_EntryTypeExtensionMap = {
		{ ".pce", EntryType::PROJECT },
		{ ".sce", EntryType::SCENE },
		{ ".ace", EntryType::ASSETREGISTRY },

		{ ".png", EntryType::IMAGE },
		{ ".jpg", EntryType::IMAGE },
		{ ".jpeg", EntryType::IMAGE },

		{ ".obj", EntryType::MODEL },
		{ ".mtl", EntryType::MATERIAL },

		{ ".glsl", EntryType::SHADER },
		{ ".ttf", EntryType::FONT },
		
		{ "", EntryType::MISC },
	};

	FileSystem::EntryType FileSystem::getEntryType(C78E::FilePath filepath) {
		if (std::filesystem::is_directory(filepath))
			return EntryType::DIRECTORY;
		auto it = c_EntryTypeExtensionMap.find(filepath.extension().string());
		if (it != c_EntryTypeExtensionMap.end())
			return it->second;
		else return EntryType::MISC;
	}

	std::string FileSystem::stringFromEntryType(EntryType type) {
		switch (type) {
		case C78E::FileSystem::DIRECTORY: return "Directory";
		case C78E::FileSystem::PROJECT: return "Project";
		case C78E::FileSystem::SCENE: return "Scene";
		case C78E::FileSystem::ASSETREGISTRY: return "AssetRegistry";
		case C78E::FileSystem::IMAGE: return "Image";
		case C78E::FileSystem::SHADER: return "Shader";
		case C78E::FileSystem::MODEL: return "Model";
		case C78E::FileSystem::MESH: return "Mesh";
		case C78E::FileSystem::MATERIAL: return "Material";
		case C78E::FileSystem::FONT: return "Font";
		case C78E::FileSystem::MISC: return "File";
		default:  return "Illegal FileType";
		}
	}

	bool FileSystem::exists(FilePath path) {
		return std::filesystem::exists(path);
	}

	FilePath FileSystem::createDirectoryIfNotPresent(FilePath directoryPath) {
		C78_CORE_ASSERT(std::filesystem::is_directory(directoryPath), "FileSystem::createDirectoryIfNotPresent: DirectoryPath is not a Directory!");
		if (!std::filesystem::exists(directoryPath))
			std::filesystem::create_directories(directoryPath);
		return directoryPath;
	}

	FilePath FileSystem::createFileIfNotPresent(FilePath filePath) {
		C78_CORE_ASSERT(std::filesystem::is_regular_file(filePath), "FileSystem::createDirectoryIfNotPresent: DirectoryPath is not a Directory!");
		if (!std::filesystem::exists(filePath)) {
			if(filePath.has_parent_path()) std::filesystem::create_directories(filePath.parent_path());
			std::ofstream output(filePath); //create File
		}
		return filePath;
	}


	bool FileSystem::isDirectory(C78E::FilePath filepath) {
		return std::filesystem::is_directory(filepath);
	}

	bool FileSystem::isFile(C78E::FilePath filepath) {
		return std::filesystem::is_regular_file(filepath);
	}


	Buffer FileSystem::readFileBinary(const FilePath& filepath) {
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream) {
			C78_CORE_ERROR("FileSystem::readFileBinary: Failed to read file: {}", filepath.string());
			C78_CORE_ASSERT(false);
			return {};
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint64_t size = end - stream.tellg();

		if (size == 0) {
			C78_CORE_WARN("FileSystem::readFileBinary: Tried to read empty file: {}", filepath.string());
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.as<char>(), size);
		stream.close();
		return buffer;
	}

}
