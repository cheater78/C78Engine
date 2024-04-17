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
			Stacked, //TODO
			Search
		};

		enum FileViewMode {
			View,
			Open,
			Save
		};

	public:
		FileView() = delete;
		FileView(FileHistory& history, FileAssets& assets, std::function<void(FilePath)> onFileClick = nullptr);
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
		std::function<void(FilePath)> m_OnFileClick = nullptr;
	};

	class FileViewGrid : public FileView {
	public:
		FileViewGrid() = delete;
		FileViewGrid(FileHistory& history, FileAssets& assets, std::function<void(FilePath)> onFileClick = nullptr);
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
		FileViewList(FileHistory& history, FileAssets& assets, std::function<void(FilePath)> onFileClick = nullptr);
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
		SearchFileView(FileManager* fileManager, FileHistory& history, FileAssets& assets, const FileSearcher::Result& result, std::function<void(FilePath)> onFileClick = nullptr);
		SearchFileView(const SearchFileView& other) = delete;
		~SearchFileView();

		virtual void show() override;
		virtual FileViewType getType() override { return Search; }
	private:
		FileManager* m_FileManager;
		FileSearcher::Result m_Result;
		std::unordered_map<FilePath, ViewFileMeta> m_Files;
	};

	class OpenFileBar {
	public:
		OpenFileBar() = delete;
		OpenFileBar(FileSystem::EntryType type);
		OpenFileBar(const OpenFileBar& other) = delete;
		~OpenFileBar();

		void show();
		bool ready() const;
		void setResult(FilePath file);
		FilePath getResult();
	private:
		FileSystem::EntryType m_Type; // filter?
		bool m_Ready = false;

		Gui::TextInput m_FileInput;
		Gui::TextButton m_OpenButton;
		Gui::TextButton m_CancelButton;
	};

	class SaveFileBar {
	public:
		SaveFileBar() = delete;
		SaveFileBar(const std::string& extension);
		SaveFileBar(const SaveFileBar& other) = delete;
		~SaveFileBar();

		void show();
		bool ready() const;
		void setResult(FilePath file);
		FilePath getResult();
	private:
		std::string m_Extension; // filter?
		bool m_Ready = false;
		
		Gui::TextInput m_FileInput;
		Gui::TextInput m_DirectoryInput;
		Gui::TextButton m_SaveButton;
		Gui::TextButton m_CancelButton;
	};

}
