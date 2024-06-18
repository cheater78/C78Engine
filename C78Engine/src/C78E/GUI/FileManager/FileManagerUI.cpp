#include "C78ePCH.h"
#include "FileManagerUI.h"

#include "FileManager.h"
#include "FileManagerAssets.h"

namespace C78E::GUI {

	const FileView::SortFilter FileView::SortNoFilter = {
			FileSystem::EntryType::DIRECTORY,

			FileSystem::EntryType::PROJECT,
			FileSystem::EntryType::SCENE,
			FileSystem::EntryType::ASSETREGISTRY,

			FileSystem::EntryType::IMAGE,
			FileSystem::EntryType::SHADER,

			FileSystem::EntryType::MODEL,
			FileSystem::EntryType::MATERIAL,
			FileSystem::EntryType::MESH,

			FileSystem::EntryType::FONT,
			FileSystem::EntryType::MISC
	};

	/*
	* abstract FileManagerUIElement
	*/
	FileManagerUIElement::FileManagerUIElement(Ref<FileManager> fileManager)
		: Element(), m_FileManager(fileManager)
	{ }

	Ref<FileManager> FileManagerUIElement::getFileManager() const { return m_FileManager; }
	Ref<FileHistory> FileManagerUIElement::getFileManagerHistory() const { return m_FileManager->m_History; }
	Ref<FileManagerAssets> FileManagerUIElement::getFileManagerAssets() const { return m_FileManager->m_Assets; }
	Ref<FileManagerUIWindow> FileManagerUIElement::getFileManagerUIWindow() const { return m_FileManager->m_UIWindow; }

	/*
	* FileSearchBar
	*/

	FileSearchBar::FileSearchBar(Ref<FileManager> fileManager)
		: FileManagerUIElement(fileManager) {

		m_SearchInput = createRef<GUI::TextInput>("Search");

		m_SearchButton = createRef<GUI::ImageButton>("", GUI::LabelPostition::NONE, "Search", getFileManagerAssets()->getUIconHandle(FileManagerIcon::SEARCH),
			[this](void) -> void {
				getFileManager()->createSearch(m_SearchInput->getContent());
			}
		);
		m_CancelSearchButton = createRef<GUI::ImageButton>("", GUI::LabelPostition::NONE, "Cancel", getFileManagerAssets()->getUIconHandle(FileManagerIcon::HOME),
			[this](void) -> void {
				getFileManager()->destroySearch();
				//m_SearchInput.setContent(""); // maybe... maybe not.. idk
			}
		);

		m_Style.itemSpacing = { 0.f, 0.f };
	}

	FileSearchBar::~FileSearchBar() { }

	void FileSearchBar::show() {
		static const float searchBarLength = 0.85f;
		static const float searchButtonLength = 0.15f;
		const float absItemWidth = GUI::getLeftOverWindowSpace().x;

		if (absItemWidth > style().parentRelativeItemWidth * GUI::getFullWindowSpace().x)
			C78_CORE_ERROR("FileSearchBar::show: getLeftOverWindowSpace does not work like u wished!");

		m_SearchInput->style().parentRelativeItemWidth = searchBarLength * absItemWidth;
		m_SearchButton->style().parentRelativeItemWidth = searchButtonLength * absItemWidth;
		m_CancelSearchButton->style().parentRelativeItemWidth = searchButtonLength * absItemWidth;
		
		m_SearchInput->show();
		GUI::SameLine();
		if (getFileManager()->hasSearch())
			m_CancelSearchButton->show();
		else m_SearchButton->show();
	}


	/*
	* FileNavBar
	*/

	FileNavBar::FileNavBar(Ref<FileManager> fileManager)
		: FileManagerUIElement(fileManager) {

		m_BackButton = createRef<ImageButton>("", GUI::LabelPostition::NONE, "Back", getFileManagerAssets()->getUIconHandle(FileManagerIcon::BACKWARD),
			[this](void) -> void {
				if (getFileManager()->hasSearch())
					getFileManager()->destroySearch();
				else {
					if (getFileManagerHistory()->canCDBackward())
						getFileManagerHistory()->cdBackward();
				}
			}
		);
		m_ForwardButton = createRef<ImageButton>("", GUI::LabelPostition::NONE, "Forward", getFileManagerAssets()->getUIconHandle(FileManagerIcon::FORWARD),
			[this](void) -> void {
				if (getFileManager()->hasSearch());
				else {
					if (getFileManagerHistory()->canCDForward())
						getFileManagerHistory()->cdForward();
				}
			}
		);
		m_HomeButton = createRef<ImageButton>("", GUI::LabelPostition::NONE, "Home", getFileManagerAssets()->getUIconHandle(FileManagerIcon::HOME),
			[this](void) -> void {
				if (getFileManager()->hasSearch())
					getFileManager()->destroySearch();
				getFileManagerHistory()->cd(getFileManagerHistory()->getBasePath());
			}
		);
		m_ParentButton = createRef<ImageButton>("", GUI::LabelPostition::NONE, "up to Parent Directory", getFileManagerAssets()->getUIconHandle(FileManagerIcon::PARENT),
			[this](void) -> void {
				if (getFileManager()->hasSearch())
					getFileManager()->destroySearch();
				if (getFileManagerHistory()->canCDParent())
					getFileManagerHistory()->cdParent();
			}
		);
		m_PathInput = createRef<TextInput>("", getFileManagerHistory()->getCWD().string()),
		m_PathSubmitButton = createRef<ImageButton>("", GUI::LabelPostition::NONE, "change Directory", getFileManagerAssets()->getUIconHandle(FileManagerIcon::FORWARD),
			[this](void) -> void {
				if (getFileManager()->hasSearch())
					getFileManager()->destroySearch();
				if (getFileManagerHistory()->canCD(m_PathInput->getContent()))
					getFileManagerHistory()->cd(m_PathInput->getContent());
			});

	}

	FileNavBar::~FileNavBar() { }

	void FileNavBar::show() {
		glm::vec2 region = GUI::getLeftOverWindowSpace();
		uint32_t shorterSide = static_cast<uint32_t>(getShorterScreenSideLength());
		glm::vec2 iconSize = resizeToScale(glm::vec2{ shorterSide, shorterSide }) * 0.017f; // make Icons 1.7% of ScreenHeight

		/*
		* for all of them
		
		if (getFileManagerHistory()->canCDBackward() || getFileManager()->hasSearch())
			m_BackButton->style().tintColor = GUI::deacTint();
		else m_BackButton->style().tintColor = GUI::noTint();

		*/

		m_BackButton->show();
		GUI::SameLine();
		m_ForwardButton->show();
		GUI::SameLine();
		m_HomeButton->show();
		GUI::SameLine();
		m_ParentButton->show();
		GUI::SameLine();

		//if (relWidth > 0.f) ImGui::PushItemWidth(region.x * relWidth - 5 * iconSize.x);

		m_PathInput->show();

		//if (relWidth > 0.f) ImGui::PopItemWidth();

		GUI::SameLine();
		if (getFileManagerHistory()->canCD(m_PathInput->getContent()))
			m_PathSubmitButton->show();
		else m_PathSubmitButton->show();

		if (!ImGui::IsItemFocused()) { // Only Update when PathInput and Submit aren't focused 
			if (getFileManagerHistory()->getCWD().string() != m_PathInput->getContent())
				m_PathInput->setContent(getFileManagerHistory()->getCWD().string());
		}

	}


	/*
	* FilePoIPanel
	*/
	FilePoIPanel::FilePoIPanel(Ref<FileManager> fileManager)
		: FileManagerUIElement(fileManager) {
	}

	FilePoIPanel::~FilePoIPanel() { }

	void FilePoIPanel::show() {
		const float fileIconSizeRel = 0.015f; // make FileCards 1.5% of shorterScreenSide
		const float shorterScreenSide = getShorterScreenSideLength();

		const glm::vec2 fileIconSize = resizeToScale(glm::vec2{ shorterScreenSide, shorterScreenSide }) * fileIconSizeRel;
		
		if (!m_Directories.empty() && std::filesystem::relative(std::next(std::begin(m_Directories), 0)->first, getFileManagerHistory()->getBasePath()).empty()) { // directory changed
			m_Directories.clear();
			m_ShowChildrenButtons.clear();
			m_ShowChildren.clear();
		}

		showDir(getFileManagerHistory()->getBasePath());
		if(m_ShowChildren.at(getFileManagerHistory()->getBasePath()))
			showDirRecurse(getFileManagerHistory()->getBasePath(), 1);
	}

	bool FilePoIPanel::showDir(FilePath directory, uint32_t depth) {
		if (C78E::FileSystem::getEntryType(directory) != FileSystem::EntryType::DIRECTORY) return false;

		if (!m_Directories.contains(directory)) {
			m_Directories.emplace(std::piecewise_construct, std::forward_as_tuple(directory),
				std::forward_as_tuple(
					directory.filename().string(), GUI::LabelPostition::RIGHT,
					"", getFileManagerAssets()->getIconHandle(directory),
					[this, directory](void) -> void {
						getFileManagerHistory()->cd(directory);
					}
				)
			);

			m_ShowChildren.emplace(directory, (depth) ? false : true);

			m_ShowChildrenButtons.emplace(std::piecewise_construct, std::forward_as_tuple(directory),
				std::forward_as_tuple(
					std::vector<std::string>{ "v", "^" },
					std::vector<std::function<void()>>{
						[this, directory](void) -> void {
							m_ShowChildren.at(directory) = true;
						},
						[this, directory](void) -> void {
							m_ShowChildren.at(directory) = false;
						}
					},
					(m_ShowChildren.at(directory)) ? 1 : 0
				)
			);

		}

		// TODO: styling
		//m_Directories.at(directory).style().*


		//ImGui::Dummy(ImVec2{ fileIconSize.x * depth ,0.f});
		ImGui::SameLine();
		m_Directories.at(directory).show();
		ImGui::SameLine();
		m_ShowChildrenButtons.at(directory).show();

		return true;
	}

	void FilePoIPanel::showDirRecurse(FilePath directory, uint32_t depth) {
		if (C78E::FileSystem::getEntryType(directory) != FileSystem::EntryType::DIRECTORY) return;
		for (FilePath file : std::filesystem::directory_iterator(directory)) {
			if(showDir(file, depth) && m_ShowChildren.at(file))
				showDirRecurse(file, depth + 1);
		}
	}


	/*
	* FileView
	*/
	FileView::FileView(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick)
		: FileManagerUIElement(fileManager), m_Filter(SortNoFilter), m_OnFileClick(onFileClick) {
	}

	FileView::~FileView() { }

	void FileView::onFileClick(FilePath file) {
		C78_ERROR("FileView::onFileClick: Default Opening Files isn't supported yet!");
		C78_ERROR("  File: {}", file.string());
	}

	/*
	* FileViewGrid
	*/
	FileViewGrid::FileViewGrid(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick)
		: FileView(fileManager, onFileClick) {
	}

	FileViewGrid::~FileViewGrid() { }

	void FileViewGrid::show() {
		glm::vec2 region = GUI::getLeftOverWindowSpace();
		const float shorterScreenSide = getShorterScreenSideLength();
		glm::vec2 fileCardSize = resizeToScale(glm::vec2{ shorterScreenSide, shorterScreenSide }) * 0.06f; // make FileCards 3% of shorterScreenSide
		uint32_t horizontalElementCount = static_cast<uint32_t>(region.x / (fileCardSize.x + 0 /*padding*/));

		if (ImGui::BeginTable("FileGrid", horizontalElementCount, ImGuiTableFlags_None)) {
			for (C78E::FileSystem::EntryType type : m_Filter)
				for (FilePath file : std::filesystem::directory_iterator(getFileManagerHistory()->getCWD())) {
					if (C78E::FileSystem::getEntryType(file) != type) continue;
					
					// Asset will get loaded here on first Call -> TODO: async bc slow
					if (!m_FileCards.contains(file)) {
						m_FileCards.emplace(std::piecewise_construct, std::forward_as_tuple(file),
							std::forward_as_tuple(
								file.filename().string(), GUI::LabelPostition::BELOW,
								"", getFileManagerAssets()->getIconHandle(file),
								[this, file](void) -> void {
									if (std::filesystem::is_directory(file))
										getFileManagerHistory()->cd(file);
									if (m_OnFileClick)
										m_OnFileClick(file);
									else FileView::onFileClick(file);
								}
							)
						);
					}

					ImGui::TableNextColumn();
					m_FileCards.at(file).show(); //TODO styling
				}
			ImGui::EndTable();
		}
	}
	
	/*
	* FileViewList
	*/
	FileViewList::FileViewList(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick)
		: FileView(fileManager, onFileClick) {
	}

	FileViewList::~FileViewList() { }

	void FileViewList::show() {
		glm::vec2 region = GUI::getLeftOverWindowSpace();
		const float shorterScreenSide = getShorterScreenSideLength();
		glm::vec2 fileIconSize = resizeToScale(glm::vec2{ shorterScreenSide, shorterScreenSide }) *0.015f; // make FileCards 1.5% of ScreenHeight
		
		if (!m_Files.empty() && std::next(std::begin(m_Files), 0)->first.parent_path() != getFileManagerHistory()->getCWD()) { // directory changed
			m_Files.clear();
		}

		if (ImGui::BeginTable("FileTable", 4, ImGuiTableFlags_None)) {
			for (C78E::FileSystem::EntryType type : m_Filter)
				for (FilePath file : std::filesystem::directory_iterator(getFileManagerHistory()->getCWD())) {
					if (C78E::FileSystem::getEntryType(file) != type) continue;

					// Asset will get loaded here on first Call -> TODO: async bc slow
					if (!m_Files.contains(file)) {
						m_Files.emplace(file, FileViewEntryMeta{
								createScope<GUI::ImageButton>(
									file.filename().string(), GUI::LabelPostition::RIGHT,
									"", getFileManagerAssets()->getIconHandle(file),
									[this, file](void) -> void {
										if (FileSystem::getEntryType(file) == FileSystem::EntryType::DIRECTORY)
											getFileManagerHistory()->cd(file);
										if (m_OnFileClick)
											m_OnFileClick(file);
										else FileView::onFileClick(file);
									}
								),
								FileSystem::getEntryType(file),
								(std::filesystem::is_directory(file)) ? 0 : std::filesystem::file_size(file),
								std::filesystem::last_write_time(file)
							}
						);
					}

					FileViewEntryMeta& filemeta = m_Files.at(file);

					ImGui::TableNextColumn(); // File Icon and Name
					filemeta.fileButton->show(); //TODO: styling
					ImGui::TableNextColumn(); // File Size
					std::string displayedFileSize = "";
					if (filemeta.fileSize != 0)
						displayedFileSize = std::to_string(filemeta.fileSize);
					ImGui::Text(displayedFileSize.c_str());
					ImGui::TableNextColumn();  // File Type
					ImGui::Text(FileSystem::stringFromEntryType(filemeta.type).c_str());
					ImGui::TableNextColumn();  // File Change Date
					ImGui::Text(std::format("{:%d.%m.%Y %H:%M}", filemeta.lastWrite).c_str());
				}
			ImGui::EndTable();
		}
	}

	

	SearchFileView::SearchFileView(Ref<FileManager> fileManager, const FileSearcher::Result& result, std::function<void(FilePath)> onFileClick)
		: FileView(fileManager, onFileClick), m_Result(result) {
	}

	SearchFileView::~SearchFileView() { }

	void SearchFileView::show() {
		glm::vec2 region = GUI::getLeftOverWindowSpace();
		const float shorterScreenSide = getShorterScreenSideLength();
		glm::vec2 fileIconSize = resizeToScale(glm::vec2{ shorterScreenSide, shorterScreenSide }) * 0.015f; // make FileCards 1.5% of ScreenHeight

		if (!m_Files.empty() && std::next(std::begin(m_Files), 0)->first.parent_path() != getFileManagerHistory()->getCWD()) { // directory changed
			m_Files.clear();
		}

		if (ImGui::BeginTable("FileTable", 5, ImGuiTableFlags_None)) {
			for (C78E::FileSystem::EntryType type : m_Filter)
				for (FilePath file : m_Result) {
					if (C78E::FileSystem::getEntryType(file) != type) continue;

					// Asset will get loaded here on first Call -> TODO: async bc slow
					if (!m_Files.contains(file)) {
						m_Files.emplace(file, FileViewEntryMeta{
								createScope<GUI::ImageButton>(
									file.filename().string(), GUI::LabelPostition::RIGHT,
									"", getFileManagerAssets()->getIconHandle(file),
									[this, file](void) -> void {
										if (FileSystem::getEntryType(file) == FileSystem::EntryType::DIRECTORY) {
											getFileManagerHistory()->cd(file);
											if (getFileManager()->hasSearch())
												getFileManager()->destroySearch();
										}
										if (m_OnFileClick)
											m_OnFileClick(file);
										else FileView::onFileClick(file);
									}
								),
								FileSystem::getEntryType(file),
								(std::filesystem::is_directory(file)) ? 0 : std::filesystem::file_size(file),
								std::filesystem::last_write_time(file)
							}
						);
					}


					FileViewEntryMeta& filemeta = m_Files.at(file);

					ImGui::TableNextColumn(); // File Icon and Name
					filemeta.fileButton->show(); //TODO: styling
					ImGui::TableNextColumn(); // File ParentDirectory
					ImGui::Text(file.parent_path().string().c_str());
					ImGui::TableNextColumn(); // File Size
					std::string displayedFileSize = "";
					if (filemeta.fileSize != 0)
						displayedFileSize = std::to_string(filemeta.fileSize);
					ImGui::Text(displayedFileSize.c_str());
					ImGui::TableNextColumn();  // File Type
					ImGui::Text(FileSystem::stringFromEntryType(filemeta.type).c_str());
					ImGui::TableNextColumn();  // File Change Date
					ImGui::Text(std::format("{:%d.%m.%Y %H:%M}", filemeta.lastWrite).c_str());
				}
			ImGui::EndTable();
		}

	}

	/*
	* FileViewStack
	*/
	FileViewStack::FileViewStack(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick)
		: FileView(fileManager, onFileClick)
	{ }

	FileViewStack::~FileViewStack()
	{ }

	void FileViewStack::show() {
		C78_CORE_ERROR("FileViewStack::show: impl missing!");
	}

	/*
	* OpenFileBar
	*/
	OpenFileBar::OpenFileBar(Ref<FileManager> fileManager, FileSystem::EntryType type)
		: FileManagerUIElement(fileManager), m_Type(type), m_Ready(false) {
		m_FileInput = createRef<TextInput>("Open:");
		m_OpenButton = createRef<TextButton>("Open", [this](void) -> void { m_Ready = true; });
		m_CancelButton = createRef<TextButton>("Cancel", [this](void) -> void { m_FileInput->setContent(""); m_Ready = true; });
	}

	OpenFileBar::~OpenFileBar() { }

	void OpenFileBar::show() {
		if (!m_Visible) return;
		begin();
		m_FileInput->show();

		m_OpenButton->show();
		GUI::SameLine();
		m_CancelButton->show();
		end();
	}

	bool OpenFileBar::ready() const { return m_Ready; }

	void OpenFileBar::setResult(FilePath file) {
		if(FileSystem::getEntryType(file) == m_Type)
			m_FileInput->setContent(file.string());
	}

	FilePath OpenFileBar::getResult() {
		return FilePath(m_FileInput->getContent());
	}




	/*
	* SaveFileBar
	*/
	SaveFileBar::SaveFileBar(Ref<FileManager> fileManager, const std::string& extension)
		: FileManagerUIElement(fileManager), m_Extension(extension), m_Ready(false) {
		m_FileInput = createRef<TextInput>("Save:");
		m_SaveButton = createRef<TextButton>("Save", [this](void) -> void { m_Ready = true; });
		m_CancelButton = createRef<TextButton>("Cancel", [this](void) -> void { m_FileInput->setContent(""); m_Ready = true; });
	}

	SaveFileBar::~SaveFileBar() { }

	void SaveFileBar::show() {
		m_FileInput->show();

		m_SaveButton->show();
		GUI::SameLine();
		m_CancelButton->show();
	}

	bool SaveFileBar::ready() const { return m_Ready; }

	void SaveFileBar::setResult(FilePath file) {
		if(file.has_extension() && file.extension() == m_Extension)
			m_FileInput->setContent(file.string());
	}

	FilePath SaveFileBar::getResult() {
		return FilePath(m_FileInput->getContent());
	}

	/*
	* OpenFileWidget
	*/
	OpenFileWidget::OpenFileWidget(FileManagerUIOpen& fileManagerUIOpen, FileSystem::EntryType entryType, FilePath defaultFilePath, const std::string& title)
		: Element(), m_FileManagerUIOpen(fileManagerUIOpen), m_EntryType(entryType), m_Title(title) {

		m_WidgetFileTI = createRef<TextInput>(m_Title, defaultFilePath.string());
		m_WidgetFileTB = createRef<TextButton>("...", [this]() { m_FileManagerUIOpen.setVisible(true); });
	}

	OpenFileWidget::~OpenFileWidget()
	{ }

	void OpenFileWidget::show() {
		if (!m_Visible) return;
		begin();
		m_WidgetFileTI->show();
		C78E::GUI::SameLine();
		m_WidgetFileTB->show();
		end();
	}

	void OpenFileWidget::setResult(FilePath file) { m_WidgetFileTI->setContent(file.string()); }
	FilePath OpenFileWidget::getResult() const { return m_WidgetFileTI->getContent(); }

	/*
	* SaveFileWidget
	*/

	SaveFileWidget::SaveFileWidget(FileManagerUISave& fileManagerUISave, const std::string& extension, FilePath defaultFilePath, const std::string& directoryTitle, bool dedicatedFileNameInput, const std::string& filenameTitle)
		: m_FileManagerUISave(fileManagerUISave), m_Extension(extension), m_DirectoryTitle(directoryTitle), m_FileNameTitle(filenameTitle) {

		m_WidgetFileNameTI = createRef<TextInput>(m_FileNameTitle, defaultFilePath.filename().string()); //TODO: validate saving folders
		m_WidgetFileNameTI->setVisible(dedicatedFileNameInput);
		
		if(m_WidgetFileNameTI->isVisible())
			m_WidgetFileTI = createRef<TextInput>(m_DirectoryTitle, defaultFilePath.parent_path().string());
		else
			m_WidgetFileTI = createRef<TextInput>(m_DirectoryTitle, defaultFilePath.string());
		m_WidgetFileTB = createRef<TextButton>("...", [this]() { m_FileManagerUISave.setVisible(true); });
	}

	void SaveFileWidget::show() {
		if (!m_Visible) return;
		begin();
		m_WidgetFileNameTI->show();
		m_WidgetFileTI->show();
		C78E::GUI::SameLine();
		m_WidgetFileTB->show();
		end();
	}

	void SaveFileWidget::setResult(FilePath file) {
		m_WidgetFileNameTI->setContent(file.filename().string()); //TODO: validate saving folders
		if (m_WidgetFileNameTI->isVisible()) {
			m_WidgetFileTI->setContent(file.parent_path().string());
		} else {
			m_WidgetFileTI->setContent(file.string());
		}
	}

	FilePath SaveFileWidget::getResult() const {
		if (m_WidgetFileNameTI->isVisible()) {
			return (FilePath)(m_WidgetFileTI->getContent()) / (FilePath)(m_WidgetFileNameTI->getContent());
		}
		else {
			return (FilePath)(m_WidgetFileTI->getContent());
		}
	}

	

	/*
	* FileManagerUIWindow
	*/
	FileManagerUIWindow::FileManagerUIWindow(Ref<FileManager> fileManager) 
		: m_FileManager(fileManager)
	{ }
	
	Ref<FileManager> FileManagerUIWindow::getFileManager() const { return m_FileManager; }
	Ref<FileHistory> FileManagerUIWindow::getFileManagerHistory() const { return m_FileManager->m_History; }
	Ref<FileManagerAssets> FileManagerUIWindow::getFileManagerAssets() const { return m_FileManager->m_Assets; }
	Ref<FileManagerUIWindow> FileManagerUIWindow::getFileManagerUIWindow() const { return m_FileManager->m_UIWindow; }

	

	/*
	* FileManagerUINavigationPanel
	*/
	FileManagerUINavigationPanel::FileManagerUINavigationPanel(Ref<FileManager> fileManager, std::function<void(FilePath)> onFileClick)
		: FileManagerUIWindow(fileManager), m_OnFileClick(onFileClick) {
		m_NavBar = createRef<FileNavBar>(fileManager);
		m_SearchBar = createRef<FileSearchBar>(fileManager);
		m_PoIPanel = createRef<FilePoIPanel>(fileManager);
		m_FileView = createRef<FileViewGrid>(fileManager, onFileClick);
		m_FileViewRestore = nullptr;
	}

	void FileManagerUINavigationPanel::content() {

		m_NavBar->show(); //TODO: styling
		GUI::SameLine();
		m_SearchBar->show(); //TODO: styling

		if (ImGui::BeginTable("FileManagerSplitView", 2, ImGuiTableFlags_Resizable)) {
			ImGui::TableNextColumn();

			m_PoIPanel->show();

			ImGui::TableNextColumn();

			m_FileView->show();

			ImGui::EndTable();
		}

	}

	FileView::FileViewType FileManagerUINavigationPanel::getFileViewType() const { return m_FileView->getType(); }

	void FileManagerUINavigationPanel::setFileViewType(FileView::FileViewType viewType) {
		switch (viewType) {
		case C78E::GUI::FileView::Grid:
			m_FileView = createRef<FileViewGrid>(getFileManager(), m_OnFileClick);
			break;
		case C78E::GUI::FileView::List:
			m_FileView = createRef<FileViewList>(getFileManager(), m_OnFileClick);
			break;
		case C78E::GUI::FileView::Stacked:
			m_FileView = createRef<FileViewStack>(getFileManager(), m_OnFileClick);
			break;
		case C78E::GUI::FileView::Search:
			m_FileView = createRef<SearchFileView>(getFileManager(), FileSearcher::Result{}, m_OnFileClick);
			break;
		default:
			break;
		}
	}

	void FileManagerUINavigationPanel::createSearch(const FileSearcher::Result& result) {
		if (m_FileViewRestore) {
			m_FileView = createRef<SearchFileView>(getFileManager(), result);
		}
		else {
			m_FileViewRestore = createRef<SearchFileView>(getFileManager(), result);
			m_FileView.swap(m_FileViewRestore);
		}
	}

	void FileManagerUINavigationPanel::destroySearch() {
		if (m_FileViewRestore) {
			m_FileView.swap(m_FileViewRestore);
			m_FileViewRestore = nullptr;
		}
		else {
			C78_CORE_ERROR("FileManagerUINavigationPanel::destroySearch: m_FileViewRestore was null!(defaulting to grid)");
			setFileViewType(C78E::GUI::FileView::Grid);
		}
	}

	bool FileManagerUINavigationPanel::hasSearch() {
		return (bool)m_FileViewRestore;
	}

	
	/*
	* FileManagerUIBrowser
	*/
	FileManagerUIBrowser::FileManagerUIBrowser(Ref<FileManager> fileManager)
		: FileManagerUINavigationPanel(fileManager, [](FilePath file) -> void { C78_CORE_ERROR("FileManagerUIBrowser: no onclick behaviour!"); })
	{ }

	void FileManagerUIBrowser::content() {
		FileManagerUINavigationPanel::content();
	}

	FileManagerUIWindow::Type FileManagerUIBrowser::getType() const { return Browser; }

	

	/*
	* FileManagerUIOpen
	*/
	FileManagerUIOpen::FileManagerUIOpen(Ref<FileManager> fileManager, FileSystem::EntryType type, FilePath defaultFilePath, const std::string& widgetTitle)
		: FileManagerUINavigationPanel(fileManager, [this](FilePath file) -> void { m_OpenFileBar->setResult(file); }) {
		m_OpenFileBar = createRef<OpenFileBar>(fileManager, type);
		m_OpenFileWidget = createRef<OpenFileWidget>(*this, type, defaultFilePath, (widgetTitle.empty()) ? ((type == FileSystem::EntryType::DIRECTORY) ? "Open Directory: " : "Open File: " ) : widgetTitle);
	}

	void FileManagerUIOpen::widget() {
		m_OpenFileWidget->show();
	}

	void FileManagerUIOpen::content() {
		FileManagerUINavigationPanel::content();
		m_OpenFileBar->show();
	}

	FileManagerUIWindow::Type FileManagerUIOpen::getType() const { return Open; }

	bool FileManagerUIOpen::ready() const { return m_OpenFileBar->ready(); }
	void FileManagerUIOpen::setResult(FilePath file) { m_OpenFileBar->setResult(file); }
	FilePath FileManagerUIOpen::getResult() { return m_OpenFileBar->getResult(); }

	Ref<OpenFileBar> FileManagerUIOpen::getOpenFileBar() const { return m_OpenFileBar; }
	Ref<OpenFileWidget> FileManagerUIOpen::getOpenFileWidget() const { return m_OpenFileWidget; }


	/*
	* FileManagerUISave
	*/
	FileManagerUISave::FileManagerUISave(Ref<FileManager> fileManager, const std::string& extension, FilePath defaultFilePath, const std::string& widgetDirectoryTitle, bool dedicatedFileNameInput, const std::string& widgetFileNameTitle)
		: FileManagerUINavigationPanel(fileManager, [this](FilePath file) -> void { C78_CORE_ERROR("FileManagerUISave: no replace warning!");  m_SaveFileBar->setResult(file); }) {
		m_SaveFileBar = createRef<SaveFileBar>(fileManager, extension);
		m_SaveFileWidget = createRef<SaveFileWidget>(*this, extension, defaultFilePath, widgetDirectoryTitle, dedicatedFileNameInput, widgetFileNameTitle);
	}

	void FileManagerUISave::widget() {
		m_SaveFileWidget->show();
	}

	void FileManagerUISave::content() {
		FileManagerUINavigationPanel::content();
		m_SaveFileBar->show();
	}

	FileManagerUIWindow::Type FileManagerUISave::getType() const { return Save; }

	bool FileManagerUISave::ready() const { return m_SaveFileBar->ready(); }
	void FileManagerUISave::setResult(FilePath file) { m_SaveFileBar->setResult(file); }
	FilePath FileManagerUISave::getResult() { return m_SaveFileBar->getResult(); }

	
	

}
