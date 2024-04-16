#pragma once
#include <C78E/GUI/Elements/ImGuiUtils.h>

#include "FileHistory.h"
#include "FileAssets.h"
#include "FileSearcher.h"
#include "FileViewElements.h"


namespace C78E {

	class FileManager;

	struct UISettings {
		uint32_t screenWidth = 2560; //TODO: like just dont
		uint32_t screenHeight = 1440;
		float scale = 1.0f;
	};

	struct ViewFileMeta {
		Scope<Gui::ImageButton> fileButton;
		FileSystem::EntryType type;
		uintmax_t fileSize;
		std::filesystem::file_time_type lastWrite;
	};

	class FileSearchBar {
	public:
		FileSearchBar(FileManager* fileManager, FileHistory& history, FileAssets& assets);
		FileSearchBar(const FileSearchBar& other) = delete;
		~FileSearchBar();

		void show(float relWidth = 0.f);
	private:
		FileManager* m_FileManager;
		FileHistory& m_History;
		FileAssets& m_Assets;
		UISettings m_UISettings;
		Gui::TextInput m_SearchInput;
		Gui::ImageButton m_SearchButton;
		Gui::ImageButton m_CancelSearchButton;
	};

	class FileNavBar {
	public:
		FileNavBar(FileManager* fileManager, FileHistory& history, FileAssets& assets);
		FileNavBar(const FileNavBar& other) = delete;
		~FileNavBar();

		void show(float relWidth = 0.f);
	private:
		FileManager* m_FileManager;
		FileHistory& m_History;
		FileAssets& m_Assets;
		UISettings m_UISettings;
		Gui::ImageButton m_BackButton;
		Gui::ImageButton m_ForwardButton;
		Gui::ImageButton m_HomeButton;
		Gui::ImageButton m_ParentButton;
		Gui::TextInput m_PathInput;
		Gui::ImageButton m_PathSubmitButton;
	};

	class FilePoIPanel  {
	public:
		FilePoIPanel(FileManager* fileManager, FileHistory& history, FileAssets& assets);
		FilePoIPanel(const FilePoIPanel& other) = delete;
		~FilePoIPanel();

		void show();
	private:
		bool showDir(FilePath directory, uint32_t depth = 0);
		void showDirRecurse(FilePath directory, uint32_t depth = 0);
	private:
		FileManager* m_FileManager;
		FileHistory& m_History;
		FileAssets& m_Assets;
		UISettings m_UISettings;
		std::unordered_map<FilePath, Gui::ImageButton> m_Directories;
		std::unordered_map<FilePath, Gui::CyclingTextButton> m_ShowChildrenButtons;
		std::unordered_map<FilePath, bool> m_ShowChildren;
		glm::vec2 m_FileIconSize{ 1.f, 1.f };
	};

	class FileView { //abstract
	public:
		using SortFilter = std::vector<FileSystem::EntryType>;
		static const SortFilter SortNoFilter;

		enum FileViewType {
			Grid,
			List,
			Search
		};
	public:
		FileView() = delete;
		FileView(FileHistory& history, FileAssets& assets);
		FileView(const FileView& other) = delete;
		~FileView();

		virtual void show() = 0;
		virtual FileViewType getType() = 0;
	public:
		static void onFileClick(FilePath file);
	protected:
		FileHistory& m_History;
		FileAssets& m_Assets;
		SortFilter m_Filter;
		UISettings m_UISettings;
	};

	class FileViewGrid : public FileView {
	public:
		FileViewGrid() = delete;
		FileViewGrid(FileHistory& history, FileAssets& assets);
		FileViewGrid(const FileViewGrid& other) = delete;
		~FileViewGrid();

		virtual void show() override;
		virtual FileViewType getType() override { return Grid; }
	private:
		std::unordered_map<FilePath, Gui::ImageButton> m_FileCards;
	};

	class FileViewList : public FileView {
	public:
		FileViewList() = delete;
		FileViewList(FileHistory& history, FileAssets& assets);
		FileViewList(const FileViewList& other) = delete;
		~FileViewList();

		virtual void show() override;
		virtual FileViewType getType() override { return List; }
	private:
		std::unordered_map<FilePath, ViewFileMeta> m_Files;
	};

	class SearchFileView : public FileView {
	public:
		SearchFileView() = delete;
		SearchFileView(FileManager* fileManager, FileHistory& history, FileAssets& assets, const FileSearcher::Result& result);
		SearchFileView(const SearchFileView& other) = delete;
		~SearchFileView();

		virtual void show() override;
		virtual FileViewType getType() override { return Search; }
	private:
		FileManager* m_FileManager;
		FileSearcher::Result m_Result;
		std::unordered_map<FilePath, ViewFileMeta> m_Files;
	};

}
