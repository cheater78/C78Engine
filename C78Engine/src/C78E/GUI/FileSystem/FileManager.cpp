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

	void FileManager::showOpenElement() {
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
		static Gui::ImageButton homeButton("Home", m_Assets.getUIconHandle(FileManagerIcon::HOME),
			[this](void) -> void {
				m_History.cd(m_History.getBasePath());
			}
		);
		static Gui::ImageButton parentButton("up to Parent Directory", m_Assets.getUIconHandle(FileManagerIcon::PARENT),
			[this](void) -> void {
				if (m_History.canCDParent())
					m_History.cdParent();
			}
		);
		//static Gui::TextInput cwdTI("", m_History.getBasePath().string());



		backButton.show({ m_UISettings.topBarElementHeight, m_UISettings.topBarElementHeight });
		Gui::SameLine();
		forwButton.show({ m_UISettings.topBarElementHeight, m_UISettings.topBarElementHeight });
		Gui::SameLine();
		homeButton.show({ m_UISettings.topBarElementHeight, m_UISettings.topBarElementHeight });
		Gui::SameLine();
		parentButton.show({ m_UISettings.topBarElementHeight, m_UISettings.topBarElementHeight });
		Gui::SameLine();
		
		ImGui::Text(m_History.getCWD().string().c_str());

		//cwdTI.show();
		/*
		if (cwdTI.getContent() != m_History.getCWD().string() && m_History.canCD(cwdTI.getContent()))
			m_History.canCD(cwdTI.getContent());
		else cwdTI.setContent(m_History.getCWD().string());
		*/
	}

	void FileManager::showFileGrid() {
		if (ImGui::BeginTable("Files", 11, ImGuiTableFlags_None)) {
			
			for (C78E::FileSystem::EntryType type : m_SortFilter)
				for (auto& entry : std::filesystem::directory_iterator(m_History.getCWD())) {
					if (C78E::FileSystem::getEntryType(entry) != type) continue;

					ImGui::TableNextColumn();

					FilePath path = entry;

					if (showSingleFileEntry(path.filename().string(), m_Assets.getIcon(path), 64)) {
						if(std::filesystem::is_directory(path))
							m_History.cd(path);
					}

				}

			ImGui::EndTable();
		}
	}

	bool FileManager::showSingleFileEntry(std::string label, C78E::Ref<C78E::Texture2D> icon, uint32_t size) {
		C78_CORE_ASSERT(icon, "");
		C78E::Ref<C78E::Texture2D> texture = icon;
		float x = 1.f;
		float y = 1.f;
		if (texture->getWidth() != texture->getHeight())
			if (texture->getWidth() < texture->getHeight()) x = (float)texture->getWidth() / (float)texture->getHeight();
			else y = (float)texture->getHeight() / (float)texture->getWidth();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		bool clicked = ImGui::ImageButton(label.c_str(), (ImTextureID)texture->getRendererID(), ImVec2((uint32_t)(size * x), (uint32_t)(size * y)));
		ImGui::PopStyleVar();
		ImGui::Text(label.c_str());
		return clicked;
	}


	glm::vec2 FileManager::getContentRegionAvail() {
		ImVec2 size = ImGui::GetContentRegionAvail();
		return glm::vec2(size.x, size.y);
	}
	

}
