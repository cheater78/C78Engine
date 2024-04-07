#pragma once
#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class FileManager {
	public:
		static void init(C78E::FilePath currentProjectPath = std::filesystem::current_path());
		static void onUpdate();
		static void onImGuiRender();

		static void setCurrentProjectPath(C78E::FilePath currentProjectPath, bool cd = true);

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
			C78E::FilePath path;
			std::map<C78E::FilePath, EntryType> entries;
			std::vector<EntryType> sortFilter;

			std::counting_semaphore<1> writeIcons = std::counting_semaphore<1>(1);
			std::unordered_map<C78E::FilePath, C78E::Ref<C78E::Texture2D>> icons;

			uint32_t elementSize = 128;
		};

		

	private:
		static void showCWDNavigator();
		static void showFileNavigator();
		static void showSingleFileCard(C78E::FilePath filepath, EntryType type);
		static bool showSingleFileEntry(std::string label, C78E::Ref<C78E::Texture2D> icon, uint32_t size = s_CWDStructure.elementSize);

		static FileManager::EntryType getEntryType(C78E::FilePath filepath);
		static void updateCurrentDirectoryStructure();
		
		static C78E::Ref<C78E::Texture2D> getIcon(C78E::FilePath file);

		static void changeDirectory(C78E::FilePath newCWD);

	private:
		static C78E::FilePath s_CWD;
		static C78E::FilePath s_CurrentProjectPath;
		static C78E::FilePath s_LastCWD;
		static uint32_t s_ColumnElements;
		static DirectoryStructure s_CWDStructure;

		static C78E::Ref<C78E::Texture2D> s_FolderTexture;
		static C78E::Ref<C78E::Texture2D> s_FileTexture;
		static C78E::Ref<C78E::Texture2D> s_FileUnknownTexture;
	};

}