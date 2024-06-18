#pragma once
#include <C78E/GUI/GUIElements.h>
#include <C78E/GUI/GUIExtensions.h>
#include <C78E/GUI/GUISystems.h>
#include <C78E/GUI/GUIUtils.h>
#include <C78E/GUI/ImGui/TextureHandle.h>

#include <C78E/Core/FileSystem/FileSearcher.h>
namespace C78E {
	class FileHistory;
}

namespace C78E::GUI {

	class FileManager;
	class FileManagerAssets;
	class FileManagerUIWindow;

	/*
	* abstract FileManagerUIElement
	* base Element of FileManagerUI
	* provides access to the owning FileManager
	*/
	class FileManagerUIElement : public Element {
	public:
		FileManagerUIElement() = delete;
		FileManagerUIElement(Ref<FileManager> fileManager);
		~FileManagerUIElement() = default;

		virtual void show() override = 0;

	protected:
		virtual Ref<FileManager> getFileManager() const final;
		virtual Ref<FileHistory> getFileManagerHistory() const final;
		virtual Ref<FileManagerAssets> getFileManagerAssets() const final;
		virtual Ref<FileManagerUIWindow> getFileManagerUIWindow() const final;
	private:
		const Ref<FileManager> m_FileManager;
	};

	/*
	* FileSearchBar
	* is a FileManager Component that allows Searches through the FileManager's current State,
	* it features a TextInput for filtering and a search/cancel ImageButton to
	* start/stop the search
	* the Search itself is handled by the given FileManager
	*/
	class FileSearchBar : public FileManagerUIElement {
	public:
		FileSearchBar(Ref<FileManager> fileManager);
		FileSearchBar(const FileSearchBar& other) = delete;
		~FileSearchBar();

		virtual void show() override final;
	private:
		Ref<GUI::TextInput> m_SearchInput = nullptr;
		Ref<GUI::ImageButton> m_SearchButton = nullptr;
		Ref<GUI::ImageButton> m_CancelSearchButton = nullptr;
	};

	/*
	* FileNavBar
	* is a FileManager Component that allows easy use of the FileManagers FileHistory,
	* it features a Back, a Forward, a Home and a ToParentDir ImageButton,
	* there is a TextInput, showing and allowing changes to the FileManagers current Directory,
	* as well as a CD ImageButton to update the FileManagers current Directory to
	*  the current TextInput's value
	*/
	class FileNavBar : public FileManagerUIElement {
	public:
		FileNavBar() = delete;
		FileNavBar(Ref<FileManager> fileManager);
		FileNavBar(const FileNavBar& other) = delete;
		~FileNavBar();

		virtual void show() override final;
	private:
		Ref<GUI::ImageButton> m_BackButton = nullptr;
		Ref<GUI::ImageButton> m_ForwardButton = nullptr;
		Ref<GUI::ImageButton> m_HomeButton = nullptr;
		Ref<GUI::ImageButton> m_ParentButton = nullptr;
		Ref<GUI::TextInput> m_PathInput = nullptr;
		Ref<GUI::ImageButton> m_PathSubmitButton = nullptr;
	};

	/*
	* FilePoIPanel
	* is a FileManager Component that visualizes the FileManagers current Directory as tree,
	* as well as it does allow for easy change to neighbor directories
	* 
	* it currently only features an ImageButton per FileSystemEntry
	* 
	* TODO: Platform dependent PoIs -> C:, <*:>, C:/Users/user/ ; /, /home/user/, /media/disk/
	* TODO: Config Saved Custom PoIs
	*/
	class FilePoIPanel : public FileManagerUIElement {
	public:
		FilePoIPanel() = delete;
		FilePoIPanel(Ref<FileManager> fileManager);
		FilePoIPanel(const FilePoIPanel& other) = delete;
		~FilePoIPanel();

		virtual void show() override final;
	private:
		bool showDir(FilePath directory, uint32_t depth = 0);
		void showDirRecurse(FilePath directory, uint32_t depth = 0);
	private:
		std::unordered_map<FilePath, GUI::ImageButton> m_Directories;
		std::unordered_map<FilePath, GUI::CyclingTextButton> m_ShowChildrenButtons;
		std::unordered_map<FilePath, bool> m_ShowChildren;
	};

	/*
	* abstract FileView
	* is a FileManager Component Base, containing the base structure for FileViews, showing
	*  the contents of the FileHistories current Directory 
	*/
	class FileView : public FileManagerUIElement {
	public:
		using SortFilter = std::vector<FileSystem::EntryType>;
		static const SortFilter SortNoFilter;

		/*
		* ViewFileMeta
		* File Meta Data representation for caching
		*/
		struct FileViewEntryMeta {
			Scope<GUI::ImageButton> fileButton; // File Icon / open action button
			FileSystem::EntryType type; // File Type
			FileSize fileSize; // File Size
			FileTime lastWrite; // Last Write Time 
		};

		enum FileViewType {
			Grid,
			List,
			Stacked, //TODO: MacOS style Directory Window Instancing
			Search
		};


	public:
		FileView() = default; //TODO: shouldnt be existing, xutil needs it
		FileView(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick = nullptr);
		FileView(const FileView& other) = delete;
		~FileView();

		virtual void show() override = 0;
		virtual FileViewType getType() = 0;
	public:
		static void onFileClick(FilePath file);
	protected:
		SortFilter m_Filter;
		std::function<void(FilePath)> m_OnFileClick = nullptr;
	};

	/*
	* FileViewGrid
	* is a FileView, displaying the FileView's FileHistory's current Directory in Grid Shape,
	* each Element features an ImageButton, containing the Icon and FileName
	*/
	class FileViewGrid : public FileView {
	public:
		FileViewGrid() = delete;
		FileViewGrid(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick = nullptr);
		FileViewGrid(const FileViewGrid& other) = delete;
		~FileViewGrid();

		virtual void show() override final;
		virtual FileViewType getType() override final { return Grid; }
	private:
		std::unordered_map<FilePath, GUI::ImageButton> m_FileCards;
	};

	/*
	* FileViewList
	* is a FileView, displaying the FileView's FileHistory's current Directory in a List,
	* each Element features an ImageButton, containing the Icon and FileName,
	* as well as Text for File Size, File Type and last Write Time
	*/
	class FileViewList : public FileView {
	public:
		FileViewList() = delete;
		FileViewList(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick = nullptr);
		FileViewList(const FileViewList& other) = delete;
		~FileViewList();

		virtual void show() override final;
		virtual FileViewType getType() override { return List; }
	private:
		std::unordered_map<FilePath, FileViewEntryMeta> m_Files;
	};

	/*
	* FileViewStack (TODO)
	* is a FileView, displaying the FileView's FileHistory's current Directory in a tree of Lists,
	* each Element features an ImageButton, containing the Icon and FileName,
	* as well as Text for File Size, File Type and last Write Time
	*/
	class FileViewStack : public FileView {
	public:
		FileViewStack() = delete;
		FileViewStack(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick = nullptr);
		FileViewStack(const FileViewList& other) = delete;
		~FileViewStack();

		virtual void show() override final;
		virtual FileViewType getType() override { return Stacked; }
	private:
		std::unordered_map<FilePath, FileViewEntryMeta> m_Files;
	};

	/*
	* SearchFileView
	* is a FileView, displaying the FileManager's Search Results in a List,
	* each Element features an ImageButton, containing the Icon and FileName, 
	* as well as Text for ParentDirectory, File Size, File Type and last Write Time
	*/
	class SearchFileView : public FileView {
	public:
		SearchFileView() = delete;
		SearchFileView(Ref<FileManager> fileManager, const FileSearcher::Result& result, std::function<void(FilePath)> onFileClick = nullptr);
		SearchFileView(const SearchFileView& other) = delete;
		~SearchFileView();

		virtual void show() override final;
		virtual FileViewType getType() override { return Search; }
	private:
		FileSearcher::Result m_Result;
		std::unordered_map<FilePath, FileViewEntryMeta> m_Files;
	};

	/*
	* OpenFileBar
	* is a FileManager Component that shows the current File to open in an InputText, to allow changes
	* it also stores the Result and features Open and Cancel TextButtons
	*/
	class OpenFileBar : public FileManagerUIElement {
	public:
		OpenFileBar() = delete;
		OpenFileBar(Ref<FileManager> fileManager, FileSystem::EntryType type);
		OpenFileBar(const OpenFileBar& other) = delete;
		~OpenFileBar();

		virtual void show() override final;

		bool ready() const;
		void setResult(FilePath file);
		FilePath getResult();
	private:
		FileSystem::EntryType m_Type; // filter?
		bool m_Ready = false;

		Ref<GUI::TextInput> m_FileInput = nullptr;
		Ref<GUI::TextButton> m_OpenButton = nullptr;
		Ref<GUI::TextButton> m_CancelButton = nullptr;
	};

	/*
	* SaveFileBar
	* is a FileManager Component that shows the current File to save in an InputText, to allow changes
	* it also stores the Result and features Save and Cancel TextButtons
	*/
	class SaveFileBar : public FileManagerUIElement {
	public:
		SaveFileBar() = delete;
		SaveFileBar(Ref<FileManager> fileManager, const std::string& extension);
		SaveFileBar(const SaveFileBar& other) = delete;
		~SaveFileBar();

		virtual void show() override final;

		bool ready() const;
		void setResult(FilePath file);
		FilePath getResult();
	private:
		std::string m_Extension; // filter?
		bool m_Ready = false;
		
		Ref<GUI::TextInput> m_FileInput = nullptr;
		Ref<GUI::TextInput> m_DirectoryInput = nullptr;
		Ref<GUI::TextButton> m_SaveButton = nullptr;
		Ref<GUI::TextButton> m_CancelButton = nullptr;
	};

	class FileManagerUIOpen;
	/*
	* OpenFileWidget
	* is a FileManager Widget that has a InputText for the FilePath and a TextButton to open a OpenFileDialog
	* to open a File or Directory
	*/
	class OpenFileWidget : public Element {
	public:
		OpenFileWidget() = delete;
		OpenFileWidget(FileManagerUIOpen& fileManagerUIOpen, FileSystem::EntryType entryType, FilePath defaultFilePath, const std::string& title);
		OpenFileWidget(const OpenFileWidget& other) = delete;
		~OpenFileWidget();

		virtual void show() override final;

		void setResult(FilePath file);
		FilePath getResult() const;
	private:
		FileManagerUIOpen& m_FileManagerUIOpen;
		FileSystem::EntryType m_EntryType;
		std::string m_Title;

		Ref<TextInput> m_WidgetFileTI = nullptr;
		Ref<TextButton> m_WidgetFileTB = nullptr;
	};

	class FileManagerUISave;
	/*
	* SaveFileWidget
	* is a FileManager Widget that has a InputText for the FilePath and a TextButton to open a SaveFileDialog
	* to save a File or Directory
	* it optionally has a dedicated InputText for the filename
	*/
	class SaveFileWidget : public Element {
	public:
		SaveFileWidget() = delete;
		SaveFileWidget(FileManagerUISave& fileManagerUISave, const std::string& extension, FilePath defaultFilePath, const std::string& directoryTitle, bool dedicatedFileNameInput = false, const std::string& filenameTitle = "");
		SaveFileWidget(const SaveFileWidget& other) = delete;
		~SaveFileWidget() = default;

		virtual void show() override final;

		void setResult(FilePath file);
		FilePath getResult() const;
	private:
		FileManagerUISave& m_FileManagerUISave;
		std::string m_Extension;

		std::string m_DirectoryTitle;
		std::string m_FileNameTitle;

		Ref<TextInput> m_WidgetFileNameTI = nullptr;
		Ref<TextInput> m_WidgetFileTI = nullptr;
		Ref<TextButton> m_WidgetFileTB = nullptr;
	};






	/*
	* abstract FileManagerUIWindow
	* 
	*/
	class FileManagerUIWindow : public GUI::Window {
	public:
		enum Type {
			Browser,
			Open,
			Save
		};
	public:
		FileManagerUIWindow() = delete;
		FileManagerUIWindow(Ref<FileManager> fileManager);
		~FileManagerUIWindow() = default;

		virtual void widget() = 0;

		virtual void content() override = 0;
		virtual Type getType() const = 0;
	protected:
		virtual Ref<FileManager> getFileManager() const final;
		virtual Ref<FileHistory> getFileManagerHistory() const final;
		virtual Ref<FileManagerAssets> getFileManagerAssets() const final;
		virtual Ref<FileManagerUIWindow> getFileManagerUIWindow() const final;

	private:
		Ref<FileManager> m_FileManager = nullptr;
	};

	/*
	* abstract FileManagerUINavigationPanel
	* 
	*/
	class FileManagerUINavigationPanel : public FileManagerUIWindow {
	public:
		FileManagerUINavigationPanel() = delete;
		FileManagerUINavigationPanel(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick);
		~FileManagerUINavigationPanel() = default;

		virtual void widget() override = 0;

		virtual void content() override;
		virtual Type getType() const override = 0;

		virtual FileView::FileViewType getFileViewType() const final;
		virtual void setFileViewType(FileView::FileViewType viewType);

		virtual void createSearch(const FileSearcher::Result& result);
		virtual void destroySearch();
		virtual bool hasSearch();
	private:
		std::function<void(FilePath)> m_OnFileClick;
		Ref<FileNavBar> m_NavBar = nullptr;
		Ref<FileSearchBar> m_SearchBar = nullptr;
		Ref<FilePoIPanel> m_PoIPanel = nullptr;
		Ref<FileView> m_FileView = nullptr;
		Ref<FileView> m_FileViewRestore = nullptr;
	};

	/*
	* abstract FileManagerUIBrowser
	*
	*/
	class FileManagerUIBrowser : public FileManagerUINavigationPanel {
	public:
		FileManagerUIBrowser() = delete;
		FileManagerUIBrowser(Ref<FileManager> fileManager);
		~FileManagerUIBrowser() = default;

		virtual void widget() override {};

		virtual void content() override final;
		virtual Type getType() const override final;

	private:
	};

	/*
	* abstract FileManagerUIOpen
	*
	*/
	class FileManagerUIOpen : public FileManagerUINavigationPanel {
	public:
		FileManagerUIOpen() = delete;
		FileManagerUIOpen(
			Ref<FileManager> fileManager,
			FileSystem::EntryType type,
			FilePath defaultFilePath,
			const std::string& widgetTitle = ""
		);
		~FileManagerUIOpen() = default;

		virtual void widget() override;

		virtual void content() override final;
		virtual Type getType() const override final;

		bool ready() const;
		void setResult(FilePath file);
		FilePath getResult();

		Ref<OpenFileBar> getOpenFileBar() const;
		Ref<OpenFileWidget> getOpenFileWidget() const;
	private:
		Ref<OpenFileBar> m_OpenFileBar = nullptr;
		Ref<OpenFileWidget> m_OpenFileWidget = nullptr;
	};
	
	/*
	* abstract FileManagerUISave
	*
	*/
	class FileManagerUISave : public FileManagerUINavigationPanel {
	public:
		FileManagerUISave() = delete;
		FileManagerUISave(
			Ref<FileManager> fileManager,
			const std::string& extension,
			FilePath defaultFilePath,
			const std::string& widgetDirectoryTitle = "",
			bool dedicatedFileNameInput = false,
			const std::string& widgetFileNameTitle = ""
		);
		~FileManagerUISave() = default;

		virtual void widget() override;

		virtual void content() override final;
		virtual Type getType() const override final;

		bool ready() const;
		void setResult(FilePath file);
		FilePath getResult();

	private:
		Ref<SaveFileBar> m_SaveFileBar = nullptr;
		Ref<SaveFileWidget> m_SaveFileWidget = nullptr;
	};

}
