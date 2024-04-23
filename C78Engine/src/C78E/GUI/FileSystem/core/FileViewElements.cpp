#include "C78ePCH.h"
#include "../FileManager.h"

#include <C78E/Core/FileSystem.h>

namespace C78E {

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
	* FileSearchBar
	*/

	FileSearchBar::FileSearchBar(FileManager* fileManager, FileHistory& history, FileAssets& assets)
		: m_FileManager(fileManager), m_History(history), m_Assets(assets),
		m_SearchInput("Search"),
		m_SearchButton("", Gui::ImageButton::LabelPostition::NONE, "Search", m_Assets.getUIconHandle(FileManagerIcon::SEARCH),
			[this](void) -> void {
				m_FileManager->createSearch(m_SearchInput.getContent());
			}
		),
		m_CancelSearchButton("", Gui::ImageButton::LabelPostition::NONE, "Cancel", m_Assets.getUIconHandle(FileManagerIcon::HOME),
			[this](void) -> void {
				m_FileManager->destroySearch();
				//m_SearchInput.setContent(""); // maybe... maybe not.. idk
			}
		) {

	}

	FileSearchBar::~FileSearchBar() { }

	void FileSearchBar::show(float relWidth) {
		glm::vec2 region = Gui::getContentRegionAvail();
		uint32_t shorterSide = static_cast<uint32_t>(std::min(m_UISettings.screenWidth, m_UISettings.screenHeight));
		glm::vec2 iconSize = m_UISettings.scale * glm::vec2{ shorterSide, shorterSide } *0.017f; // make Icons 1.7% of ScreenHeight

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		if (relWidth > 0.f) {
			ImGui::PushItemWidth(region.x * relWidth - 1 * iconSize.x);
			m_SearchInput.show(static_cast<uint32_t>(region.x * relWidth - 1.f * iconSize.x));
			ImGui::PopItemWidth();
		}
		else {
			m_SearchInput.show();
		}
		Gui::SameLine();

		if (m_FileManager->hasSearch())
			m_CancelSearchButton.show(iconSize);
		else m_SearchButton.show(iconSize);

		ImGui::PopStyleVar();
	}


	/*
	* FileNavBar
	*/

	FileNavBar::FileNavBar(FileManager* fileManager, FileHistory& history, FileAssets& assets)
		: m_FileManager(fileManager), m_History(history), m_Assets(assets),
		m_BackButton("", Gui::ImageButton::LabelPostition::NONE, "Back", m_Assets.getUIconHandle(FileManagerIcon::BACKWARD),
			[this](void) -> void {
				if(m_FileManager->hasSearch())
					m_FileManager->destroySearch();
				else {
					if (m_History.canCDBackward())
						m_History.cdBackward();
				}
			}
		),
		m_ForwardButton("", Gui::ImageButton::LabelPostition::NONE, "Forward", m_Assets.getUIconHandle(FileManagerIcon::FORWARD),
			[this](void) -> void {
				if (m_FileManager->hasSearch());
				else {
					if (m_History.canCDForward())
						m_History.cdForward();
				}
			}
		),
		m_HomeButton("", Gui::ImageButton::LabelPostition::NONE, "Home", m_Assets.getUIconHandle(FileManagerIcon::HOME),
			[this](void) -> void {
				if (m_FileManager->hasSearch())
					m_FileManager->destroySearch();
				m_History.cd(m_History.getBasePath());
			}
		),
		m_ParentButton("", Gui::ImageButton::LabelPostition::NONE, "up to Parent Directory", m_Assets.getUIconHandle(FileManagerIcon::PARENT),
			[this](void) -> void {
				if (m_FileManager->hasSearch())
					m_FileManager->destroySearch();
				if (m_History.canCDParent())
					m_History.cdParent();
			}
		),
		m_PathInput("", m_History.getCWD().string()),
		m_PathSubmitButton("", Gui::ImageButton::LabelPostition::NONE, "change Directory", m_Assets.getUIconHandle(FileManagerIcon::FORWARD),
			[this](void) -> void {
				if (m_FileManager->hasSearch())
					m_FileManager->destroySearch();
				if (m_History.canCD(m_PathInput.getContent()))
					m_History.cd(m_PathInput.getContent());
			}
		) {
	}

	FileNavBar::~FileNavBar() { }

	void FileNavBar::show(float relWidth) {
		glm::vec2 region = Gui::getContentRegionAvail();
		uint32_t shorterSide = static_cast<uint32_t>(std::min(m_UISettings.screenWidth, m_UISettings.screenHeight));
		glm::vec2 iconSize = m_UISettings.scale * glm::vec2{ shorterSide, shorterSide } * 0.017f; // make Icons 1.7% of ScreenHeight

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

		if(m_History.canCDBackward() || m_FileManager->hasSearch())
			m_BackButton.show(iconSize);
		else m_BackButton.show(iconSize, Gui::autoColor(), glm::vec4{1.f, 1.f, 1.f, 0.3f});
		Gui::SameLine();
		if (m_History.canCDForward())
			m_ForwardButton.show(iconSize);
		else m_ForwardButton.show(iconSize, Gui::autoColor(), glm::vec4{ 1.f, 1.f, 1.f, 0.3f });
		Gui::SameLine();
		if (m_History.canCD(m_History.getBasePath()))
			m_HomeButton.show(iconSize);
		else m_HomeButton.show(iconSize, Gui::autoColor(), glm::vec4{ 1.f, 1.f, 1.f, 0.3f });
		Gui::SameLine();
		if (m_History.canCDParent())
			m_ParentButton.show(iconSize);
		else m_ParentButton.show(iconSize, Gui::autoColor(), glm::vec4{ 1.f, 1.f, 1.f, 0.3f });
		Gui::SameLine();

		{
			ImGui::PushID(1);

			if (relWidth > 0.f) ImGui::PushItemWidth(region.x * relWidth - 5 * iconSize.x);

			m_PathInput.show();

			if (relWidth > 0.f) ImGui::PopItemWidth();

			Gui::SameLine();
			if (m_History.canCD(m_PathInput.getContent()))
				m_PathSubmitButton.show(iconSize);
			else m_PathSubmitButton.show(iconSize, Gui::autoColor(), glm::vec4{ 1.f, 1.f, 1.f, 0.3f });

			if (!ImGui::IsItemFocused()) { // Only Update when PathInput and Submit aren't focused 
				if (m_History.getCWD().string() != m_PathInput.getContent())
					m_PathInput.setContent(m_History.getCWD().string());
			}

			ImGui::PopID();
		}

		ImGui::PopStyleVar();
	}


	/*
	* FilePoIPanel
	*/
	FilePoIPanel::FilePoIPanel(FileManager* fileManager, FileHistory& history, FileAssets& assets) 
		: m_FileManager(fileManager), m_History(history), m_Assets(assets) {
	}

	FilePoIPanel::~FilePoIPanel() { }

	void FilePoIPanel::show() {

		m_FileIconSize = m_UISettings.scale * glm::vec2{ m_UISettings.screenHeight, m_UISettings.screenHeight } *0.015f; // make FileCards 1.5% of ScreenHeight
		
		if (!m_Directories.empty() && std::filesystem::relative(std::next(std::begin(m_Directories), 0)->first, m_History.getBasePath()).empty()) { // directory changed
			m_Directories.clear();
			m_ShowChildrenButtons.clear();
			m_ShowChildren.clear();
		}

		showDir(m_History.getBasePath());
		if(m_ShowChildren.at(m_History.getBasePath()))
			showDirRecurse(m_History.getBasePath(), 1);
	}

	bool FilePoIPanel::showDir(FilePath directory, uint32_t depth) {
		if (C78E::FileSystem::getEntryType(directory) != FileSystem::EntryType::DIRECTORY) return false;

		if (!m_Directories.contains(directory)) {
			m_Directories.emplace(std::piecewise_construct, std::forward_as_tuple(directory),
				std::forward_as_tuple(
					directory.filename().string(), Gui::ImageButton::LabelPostition::RIGHT,
					"", m_Assets.getIconHandle(directory),
					[this, directory](void) -> void {
						m_History.cd(directory);
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

		ImGui::Dummy(ImVec2{ m_FileIconSize.x * depth ,0.f});
		ImGui::SameLine();
		m_Directories.at(directory).show(m_FileIconSize);
		ImGui::SameLine();
		m_ShowChildrenButtons.at(directory).show(m_FileIconSize);

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
	FileView::FileView(FileHistory& history, FileAssets& assets, std::function<void(FilePath)> onFileClick)
		: m_History(history), m_Assets(assets), m_Filter(SortNoFilter), m_UISettings(), m_OnFileClick(onFileClick) {
	}

	FileView::~FileView() { }

	void FileView::onFileClick(FilePath file) {
		C78_ERROR("FileView::onFileClick: Default Opening Files isn't supported yet!");
		C78_ERROR("  File: {}", file.string());
	}

	/*
	* FileViewGrid
	*/
	FileViewGrid::FileViewGrid(FileHistory& history, FileAssets& assets, std::function<void(FilePath)> onFileClick)
		: FileView(history, assets, onFileClick) {
	}

	FileViewGrid::~FileViewGrid() { }

	void FileViewGrid::show() {
		glm::vec2 region = Gui::getContentRegionAvail();
		glm::vec2 fileCardSize = m_UISettings.scale * glm::vec2{ m_UISettings.screenWidth, m_UISettings.screenWidth } * 0.03f; // make FileCards 6% of ScreenWidth
		uint32_t horizontalElementCount = static_cast<uint32_t>(region.x / (fileCardSize.x + 0 /*padding*/));

		if (ImGui::BeginTable("FileGrid", horizontalElementCount, ImGuiTableFlags_None)) {
			for (C78E::FileSystem::EntryType type : m_Filter)
				for (FilePath file : std::filesystem::directory_iterator(m_History.getCWD())) {
					if (C78E::FileSystem::getEntryType(file) != type) continue;
					
					// Asset will get loaded here on first Call -> TODO: async bc slow
					if (!m_FileCards.contains(file)) {
						m_FileCards.emplace(std::piecewise_construct, std::forward_as_tuple(file),
							std::forward_as_tuple(
								file.filename().string(), Gui::ImageButton::LabelPostition::BELOW,
								"", m_Assets.getIconHandle(file),
								[this, file](void) -> void {
									if (std::filesystem::is_directory(file))
										m_History.cd(file);
									if (m_OnFileClick)
										m_OnFileClick(file);
									else FileView::onFileClick(file);
								}
							)
						);
					}

					ImGui::TableNextColumn();
					m_FileCards.at(file).show(fileCardSize, Gui::autoColor(), Gui::noTintColor());
				}
			ImGui::EndTable();
		}
	}
	
	/*
	* FileViewList
	*/
	FileViewList::FileViewList(FileHistory& history, FileAssets& assets, std::function<void(FilePath)> onFileClick)
		: FileView(history, assets, onFileClick) {
	}

	FileViewList::~FileViewList() { }

	void FileViewList::show() {
		glm::vec2 region = Gui::getContentRegionAvail();
		glm::vec2 fileIconSize = m_UISettings.scale * glm::vec2{ m_UISettings.screenHeight, m_UISettings.screenHeight } *0.015f; // make FileCards 1.5% of ScreenHeight
		
		if (!m_Files.empty() && std::next(std::begin(m_Files), 0)->first.parent_path() != m_History.getCWD()) { // directory changed
			m_Files.clear();
		}

		if (ImGui::BeginTable("FileTable", 4, ImGuiTableFlags_None)) {
			for (C78E::FileSystem::EntryType type : m_Filter)
				for (FilePath file : std::filesystem::directory_iterator(m_History.getCWD())) {
					if (C78E::FileSystem::getEntryType(file) != type) continue;

					// Asset will get loaded here on first Call -> TODO: async bc slow
					if (!m_Files.contains(file)) {
						m_Files.emplace(file, ViewFileMeta{
								createScope<Gui::ImageButton>(
									file.filename().string(), Gui::ImageButton::LabelPostition::RIGHT,
									"", m_Assets.getIconHandle(file),
									[this, file](void) -> void {
										if (std::filesystem::is_directory(file))
											m_History.cd(file);
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

					ViewFileMeta& filemeta = m_Files.at(file);

					ImGui::TableNextColumn(); // File Icon and Name
					filemeta.fileButton->show(fileIconSize, Gui::autoColor(), Gui::noTintColor());
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

	

	SearchFileView::SearchFileView(FileManager* fileManager, FileHistory& history, FileAssets& assets, const FileSearcher::Result& result, std::function<void(FilePath)> onFileClick)
		: FileView(history, assets, onFileClick), m_FileManager(fileManager), m_Result(result) {
	}

	SearchFileView::~SearchFileView() { }

	void SearchFileView::show() {
		glm::vec2 region = Gui::getContentRegionAvail();
		glm::vec2 fileIconSize = m_UISettings.scale * glm::vec2{ m_UISettings.screenHeight, m_UISettings.screenHeight } *0.015f; // make FileCards 1.5% of ScreenHeight

		if (!m_Files.empty() && std::next(std::begin(m_Files), 0)->first.parent_path() != m_History.getCWD()) { // directory changed
			m_Files.clear();
		}

		if (ImGui::BeginTable("FileTable", 5, ImGuiTableFlags_None)) {
			for (C78E::FileSystem::EntryType type : m_Filter)
				for (FilePath file : m_Result) {
					if (C78E::FileSystem::getEntryType(file) != type) continue;

					// Asset will get loaded here on first Call -> TODO: async bc slow
					if (!m_Files.contains(file)) {
						m_Files.emplace(file, ViewFileMeta{
								createScope<Gui::ImageButton>(
									file.filename().string(), Gui::ImageButton::LabelPostition::RIGHT,
									"", m_Assets.getIconHandle(file),
									[this, file](void) -> void {
										if (std::filesystem::is_directory(file)) {
											m_History.cd(file);
											if (m_FileManager->hasSearch())
												m_FileManager->destroySearch();
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


					ViewFileMeta& filemeta = m_Files.at(file);

					ImGui::TableNextColumn(); // File Icon and Name
					filemeta.fileButton->show(fileIconSize, Gui::autoColor(), Gui::noTintColor());
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
	* OpenFileBar
	*/
	OpenFileBar::OpenFileBar(FileSystem::EntryType type) 
		: m_Type(type), 
		m_FileInput("Open:"),
		m_OpenButton("Open", [this](void) -> void { m_Ready = true; } ),
		m_CancelButton("Cancel", [this](void) -> void { m_FileInput.setContent(""); m_Ready = true; }) {
	}

	OpenFileBar::~OpenFileBar() { }

	void OpenFileBar::show() {
		m_FileInput.show();

		m_OpenButton.show();
		Gui::SameLine();
		m_CancelButton.show();
	}

	bool OpenFileBar::ready() const { return m_Ready; }

	void OpenFileBar::setResult(FilePath file) {
		if(FileSystem::getEntryType(file) == m_Type)
			m_FileInput.setContent(file.string());
	}

	FilePath OpenFileBar::getResult() {
		return FilePath(m_FileInput.getContent());
	}




	/*
	* SaveFileBar
	*/
	SaveFileBar::SaveFileBar(const std::string& extension) 
		: m_Extension(extension),
		m_FileInput("Save:"),
		m_SaveButton("Save", [this](void) -> void { m_Ready = true; }),
		m_CancelButton("Cancel", [this](void) -> void { m_FileInput.setContent(""); m_Ready = true; }) {
	}

	SaveFileBar::~SaveFileBar() { }

	void SaveFileBar::show() {
		m_FileInput.show();

		m_SaveButton.show();
		Gui::SameLine();
		m_CancelButton.show();
	}

	bool SaveFileBar::ready() const { return m_Ready; }

	void SaveFileBar::setResult(FilePath file) {
		if(file.has_extension() && file.extension() == m_Extension)
			m_FileInput.setContent(file.string());
	}

	FilePath SaveFileBar::getResult() {
		return FilePath(m_FileInput.getContent());
	}

}
