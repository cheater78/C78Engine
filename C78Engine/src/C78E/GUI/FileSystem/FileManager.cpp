#include "C78ePCH.h"
#include "FileManager.h"

#include <C78E/GUI/Elements/ImGuiUtils.h>

namespace C78E {

	FileManager::FileManager(FilePath defaultDirectory)
		: m_History(defaultDirectory), m_UISettings(), m_Assets("C:\\dev\\c-cpp\\C78Engine\\C78Editor\\assets\\textures\\FileManager"), m_UIView(C78E::FileManager::FileView) {

		m_SortFilter = {
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
	}

	void C78E::FileManager::onImGuiRender() {
		ImGui::Begin(m_Title.c_str());
		
		switch (m_UIView) {
		case C78E::FileManager::FileView: showFileView(); break;
		case C78E::FileManager::OpenElement: showOpenElement(); break;
		case C78E::FileManager::SaveElement: showSaveElement(); break;
		default: ImGui::Text("FileManager::onImGuiRender: Broken View!"); break;
		}

		ImGui::End();
	}

	
	void FileManager::showFileView() {
		showTopBar();
		showFileGrid();
	}

	void FileManager::showOpenElement()
	{
	}

	void FileManager::showSaveElement() {
	}

	void FileManager::showTopBar() {

		static Gui::ImageButton backButton("Back", m_Assets.getUIconHandle(FileManagerIcon::BACKWARD),
			[this](void) -> void {
				if (m_History.canCDBackward())
					m_History.cdBackward();
			}
		);
		static Gui::ImageButton forwButton("Forward", m_Assets.getUIconHandle(FileManagerIcon::FORWARD),
			[this](void) -> void {
				if (m_History.canCDForward())
					m_History.cdForward();
			}
		);
		static Gui::ImageButton parentButton("up to Parent Directory", m_Assets.getUIconHandle(FileManagerIcon::PARENT),
			[this](void) -> void {
				if (m_History.canCDParent())
					m_History.canCDParent();
			}
		);



		backButton.show({ m_UISettings.topBarElementHeight, m_UISettings.topBarElementHeight });
		Gui::SameLine();
		forwButton.show({ m_UISettings.topBarElementHeight, m_UISettings.topBarElementHeight });
		Gui::SameLine();
		parentButton.show({ m_UISettings.topBarElementHeight, m_UISettings.topBarElementHeight });
		Gui::SameLine();
		ImGui::Text(m_History.getCWD().string().c_str());
	}

	void FileManager::showFileGrid() {

	}


	glm::vec2 FileManager::getContentRegionAvail() {
		ImVec2 size = ImGui::GetContentRegionAvail();
		return glm::vec2(size.x, size.y);
	}
	

}
