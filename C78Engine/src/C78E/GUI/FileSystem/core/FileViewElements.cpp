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


	FileNavBar::FileNavBar(FileHistory& history, FileAssets& assets) 
		: m_History(history), m_Assets(assets),
		m_BackButton("", Gui::ImageButton::LabelPostition::NONE, "Back", m_Assets.getUIconHandle(FileManagerIcon::BACKWARD),
			[this](void) -> void {
				if (m_History.canCDBackward())
					m_History.cdBackward();
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

	void FileNavBar::show() {
		glm::vec2 region = Gui::getContentRegionAvail();
		uint32_t shorterSide = static_cast<uint32_t>(std::min(m_UISettings.screenWidth, m_UISettings.screenHeight));
		glm::vec2 iconSize = m_UISettings.scale * glm::vec2{ shorterSide, shorterSide } * 0.02f; // make FileCards 1.5% of ScreenHeight

		m_BackButton.show(iconSize);
		Gui::SameLine();
		m_ForwardButton.show(iconSize);
		Gui::SameLine();
		m_HomeButton.show(iconSize);
		Gui::SameLine();
		m_ParentButton.show(iconSize);
		Gui::SameLine();
		m_PathInput.show();
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
		glm::vec2 fileCardSize = m_UISettings.scale * glm::vec2{ m_UISettings.screenWidth, m_UISettings.screenWidth } * 0.06f; // make FileCards 8% of ScreenWidth
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

}
