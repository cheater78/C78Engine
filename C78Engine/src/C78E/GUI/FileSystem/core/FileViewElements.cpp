#include "C78ePCH.h"
#include "FileViewElements.h"

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

	FileSearchBar::FileSearchBar(FileHistory& history, FileAssets& assets, Ref<FileView>& fileView)
		: m_History(history), m_Assets(assets), m_FileView(fileView),
		m_SearchInput("Search"),
		m_SearchButton("", Gui::ImageButton::LabelPostition::NONE, "Search", m_Assets.getUIconHandle(FileManagerIcon::SEARCH),
			[this](void) -> void {
				FileSearcher::Result result = FileSearcher::search(m_History.getCWD(), m_SearchInput.getContent());
				m_RestoreFileView = createRef<SearchFileView>(m_History, m_Assets, result);
				m_FileView.swap(m_RestoreFileView);
			}
		),
		m_CancelSearchButton("", Gui::ImageButton::LabelPostition::NONE, "Cancel", m_Assets.getUIconHandle(FileManagerIcon::HOME),
			[this](void) -> void {
				closeFileSearch();
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

		if (m_FileView->getType() == FileView::FileViewType::Search)
			m_CancelSearchButton.show(iconSize);
		else m_SearchButton.show(iconSize);

		ImGui::PopStyleVar();
	}

	void FileSearchBar::closeFileSearch() {
		if (m_RestoreFileView) {
			m_FileView.swap(m_RestoreFileView);
			m_RestoreFileView.reset();
		}
		else if (!m_FileView) m_FileView = createRef<FileViewGrid>(m_History, m_Assets); // Emergency
	}

	FileNavBar::FileNavBar(FileHistory& history, FileAssets& assets, Ref<FileView>& fileView, FileSearchBar& searchBar)
		: m_History(history), m_Assets(assets), m_FileView(fileView), m_SearchBar(searchBar),
		m_BackButton("", Gui::ImageButton::LabelPostition::NONE, "Back", m_Assets.getUIconHandle(FileManagerIcon::BACKWARD),
			[this](void) -> void {
				if (m_FileView->getType() != FileView::FileViewType::Search) {
					if(m_History.canCDBackward())
						m_History.cdBackward();
				}
				else m_SearchBar.closeFileSearch();
			}
		),
		m_ForwardButton("", Gui::ImageButton::LabelPostition::NONE, "Forward", m_Assets.getUIconHandle(FileManagerIcon::FORWARD),
			[this](void) -> void {
				if (m_History.canCDForward())
					m_History.cdForward();
			}
		),
		m_HomeButton("", Gui::ImageButton::LabelPostition::NONE, "Home", m_Assets.getUIconHandle(FileManagerIcon::HOME),
			[this](void) -> void {
				m_History.cd(m_History.getBasePath());
			}
		),
		m_ParentButton("", Gui::ImageButton::LabelPostition::NONE, "up to Parent Directory", m_Assets.getUIconHandle(FileManagerIcon::PARENT),
			[this](void) -> void {
				if (m_History.canCDParent())
					m_History.cdParent();
			}
		),
		m_PathInput("", m_History.getCWD().string()) {
	}

	FileNavBar::~FileNavBar() { }

	void FileNavBar::show(float relWidth) {
		glm::vec2 region = Gui::getContentRegionAvail();
		uint32_t shorterSide = static_cast<uint32_t>(std::min(m_UISettings.screenWidth, m_UISettings.screenHeight));
		glm::vec2 iconSize = m_UISettings.scale * glm::vec2{ shorterSide, shorterSide } * 0.017f; // make Icons 1.7% of ScreenHeight

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

		m_BackButton.show(iconSize);
		Gui::SameLine();
		m_ForwardButton.show(iconSize);
		Gui::SameLine();
		m_HomeButton.show(iconSize);
		Gui::SameLine();
		m_ParentButton.show(iconSize);
		Gui::SameLine();

		if (m_History.getCWD().string() != m_PathInput.getContent())
			m_PathInput.setContent(m_History.getCWD().string());

		if (relWidth > 0.f) ImGui::PushItemWidth(region.x * relWidth - 4 * iconSize.x);

		m_PathInput.show();

		if (relWidth > 0.f) ImGui::PopItemWidth();

		ImGui::PopStyleVar();
	}


	/*
	* FileView
	*/
	FileView::FileView(FileHistory& history, FileAssets& assets) 
		: m_History(history), m_Assets(assets), m_Filter(SortNoFilter), m_UISettings() {
	}

	FileView::~FileView() { }

	void FileView::onFileClick(FilePath file) {
		C78_ERROR("Opening Files isn't supported yet!");
		C78_ERROR("  File: {}", file.string());
	}

	/*
	* FileViewGrid
	*/
	FileViewGrid::FileViewGrid(FileHistory& history, FileAssets& assets)
		: FileView(history, assets) {
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
	FileViewList::FileViewList(FileHistory& history, FileAssets& assets)
		: FileView(history, assets) {
	}

	FileViewList::~FileViewList() { }

	void FileViewList::show() {
		glm::vec2 region = Gui::getContentRegionAvail();
		glm::vec2 fileIconSize = m_UISettings.scale * glm::vec2{ m_UISettings.screenHeight, m_UISettings.screenHeight } *0.015f; // make FileCards 1.5% of ScreenHeight
		
		if (ImGui::BeginTable("FileTable", 4, ImGuiTableFlags_None)) {
			for (C78E::FileSystem::EntryType type : m_Filter)
				for (FilePath file : std::filesystem::directory_iterator(m_History.getCWD())) {
					if (C78E::FileSystem::getEntryType(file) != type) continue;

					// Asset will get loaded here on first Call -> TODO: async bc slow
					if (!m_FileCards.contains(file)) {
						m_FileCards.emplace(std::piecewise_construct, std::forward_as_tuple(file),
							std::forward_as_tuple(
								file.filename().string(), Gui::ImageButton::LabelPostition::RIGHT,
								"", m_Assets.getIconHandle(file),
								[this, file](void) -> void {
									if (std::filesystem::is_directory(file))
										m_History.cd(file);
									else FileView::onFileClick(file);
								}
							)
						);
					}

					ImGui::TableNextColumn(); // File Icon and Name
					m_FileCards.at(file).show(fileIconSize, Gui::autoColor(), Gui::noTintColor());
					ImGui::TableNextColumn(); // File Size
					std::string displayedFileSize = "";
					if (!std::filesystem::is_directory(file))
						displayedFileSize = std::to_string(std::filesystem::file_size(file));
					ImGui::Text(displayedFileSize.c_str());
					ImGui::TableNextColumn();  // File Type
					ImGui::Text(FileSystem::stringFromEntryType(FileSystem::getEntryType(file)).c_str());
					ImGui::TableNextColumn();  // File Change Date
					ImGui::Text(std::format("{:%d.%m.%Y %H:%M}", std::filesystem::last_write_time(file)).c_str());
				}
			ImGui::EndTable();
		}
	}

	

	SearchFileView::SearchFileView(FileHistory& history, FileAssets& assets, FileSearcher::Result& result)
		: FileView(history, assets), m_Result(result) {
	}

	SearchFileView::~SearchFileView() { }

	void SearchFileView::show() {
		glm::vec2 region = Gui::getContentRegionAvail();
		glm::vec2 fileIconSize = m_UISettings.scale * glm::vec2{ m_UISettings.screenHeight, m_UISettings.screenHeight } *0.015f; // make FileCards 1.5% of ScreenHeight

		if (ImGui::BeginTable("FileTable", 5, ImGuiTableFlags_None)) {
			for (C78E::FileSystem::EntryType type : m_Filter)
				for (FilePath file : m_Result) {
					if (C78E::FileSystem::getEntryType(file) != type) continue;

					// Asset will get loaded here on first Call -> TODO: async bc slow
					if (!m_FileCards.contains(file)) {
						m_FileCards.emplace(std::piecewise_construct, std::forward_as_tuple(file),
							std::forward_as_tuple(
								file.filename().string(), Gui::ImageButton::LabelPostition::RIGHT,
								"", m_Assets.getIconHandle(file),
								[this, file](void) -> void {
									if (std::filesystem::is_directory(file))
										m_History.cd(file);
									else FileView::onFileClick(file);
								}
							)
						);
					}

					ImGui::TableNextColumn(); // File Icon and Name
					m_FileCards.at(file).show(fileIconSize, Gui::autoColor(), Gui::noTintColor());
					ImGui::TableNextColumn(); // File ParentdDirectory
					ImGui::Text(file.parent_path().string().c_str());
					ImGui::TableNextColumn(); // File Size
					std::string displayedFileSize = "";
					if (!std::filesystem::is_directory(file))
						displayedFileSize = std::to_string(std::filesystem::file_size(file));
					ImGui::Text(displayedFileSize.c_str());
					ImGui::TableNextColumn();  // File Type
					ImGui::Text(FileSystem::stringFromEntryType(FileSystem::getEntryType(file)).c_str());
					ImGui::TableNextColumn();  // File Change Date
					ImGui::Text(std::format("{:%d.%m.%Y %H:%M}", std::filesystem::last_write_time(file)).c_str());
				}
			ImGui::EndTable();
		}
	}

}
