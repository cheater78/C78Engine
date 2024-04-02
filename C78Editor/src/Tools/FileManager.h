#pragma once
#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class FileManager {
	public:
		static void init(std::filesystem::path currentProjectPath = std::filesystem::current_path());
		static void onUpdate();
		static void onImGuiRender();

	private:
		enum EntryType {
			DIRECTORY,
			IMAGE,
			SHADER,
			MODEL,
			MATERIAL,
			FONT,
			PROJECT,
			SCENE,
			MISC
		};

		struct DirectoryStructure {
			std::filesystem::path path;
			std::map<std::filesystem::path, EntryType> entries;
			std::vector<EntryType> sortFilter;

			std::counting_semaphore<1> writeIcons = std::counting_semaphore<1>(1);
			std::unordered_map<std::filesystem::path, C78E::Ref<C78E::Texture2D>> icons;

			uint32_t elementSize = 128;
		};

	private:
		static void showCWDNavigator();
		static void showFileNavigator();
		static void showSingleFileCard(std::filesystem::path filepath, EntryType type);
		static bool showSingleFileEntry(std::string label, C78E::Ref<C78E::Texture2D> icon, uint32_t size = s_CWDStructure.elementSize);

		static FileManager::EntryType getEntryType(std::filesystem::path filepath);
		static void updateCurrentDirectoryStructure();
		
		static C78E::Ref<C78E::Texture2D> getIcon(std::filesystem::path file);

		static void changeDirectory(std::filesystem::path newCWD);

	private:
		static std::filesystem::path s_CWD;
		static std::filesystem::path s_CurrentProjectPath;
		static std::filesystem::path s_LastCWD;
		static uint32_t s_ColumnElements;
		static DirectoryStructure s_CWDStructure;

		static C78E::Ref<C78E::Texture2D> s_FolderTexture;
		static C78E::Ref<C78E::Texture2D> s_FileTexture;
		static C78E::Ref<C78E::Texture2D> s_FileUnknownTexture;
	};

}