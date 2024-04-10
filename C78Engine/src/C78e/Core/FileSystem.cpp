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
