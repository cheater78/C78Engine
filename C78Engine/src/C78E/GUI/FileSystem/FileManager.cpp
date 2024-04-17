#include "C78ePCH.h"
#include "FileManager.h"

#include <C78E/GUI/Elements/ImGuiUtils.h>

namespace C78E {

	FileManager::FileManager(FilePath defaultDirectory)
		: m_UISettings(),
		m_History("C:\\dev\\c-cpp\\C78Engine\\C78Project"),
		m_Assets("C:\\dev\\c-cpp\\C78Engine\\C78Editor\\assets\\textures\\FileManager"),
		m_FileView(createScope<FileViewGrid>(m_History, m_Assets)),
		m_SearchBar(this, m_History, m_Assets),
		m_NavBar(this, m_History, m_Assets),
		m_PoIPanel(this, m_History, m_Assets),
		m_UIView(C78E::FileManager::ShowFiles) {

	}

	void FileManager::setOpenDialog(FileSystem::EntryType type) {
		m_OpenFileBar = createScope<OpenFileBar>(type);
		m_UIView = OpenElement;
		if (hasSearch()) // should never happen, this function should be called right after creatiion of FileManager
			destroySearch();
		m_FileView = createScope<FileViewGrid>(m_History, m_Assets, [this](FilePath file) -> void { m_OpenFileBar->setResult(file); });
		m_OpenFileBar->setResult(m_History.getCWD());
	}

	void FileManager::setSaveDialog(const std::string& extension) {
		m_SaveFileBar = createScope<SaveFileBar>(extension);
		m_UIView = SaveElement;
		if (hasSearch()) // should never happen, this function should be called right after creatiion of FileManager
			destroySearch();
		m_FileView = createScope<FileViewGrid>(m_History, m_Assets, [this](FilePath file) -> void { m_SaveFileBar->setResult(file); });
		m_SaveFileBar->setResult(m_History.getCWD());
	}

	bool FileManager::dialogReady() const {
		switch (m_UIView) {
		case C78E::FileManager::ShowFiles: return false;
		case C78E::FileManager::OpenElement: return m_OpenFileBar->ready();
		case C78E::FileManager::SaveElement: return m_SaveFileBar->ready();
		default: return false;
		}
	}

	FilePath FileManager::getDialogResult() {
		switch (m_UIView) {
		case C78E::FileManager::ShowFiles: return FilePath();
		case C78E::FileManager::OpenElement: return m_OpenFileBar->getResult();
		case C78E::FileManager::SaveElement: return m_SaveFileBar->getResult();
		default: return FilePath();
		}
	}

	void FileManager::createSearch(const std::string& searchDirective) {
		m_RestoreFileView = createScope<SearchFileView>(this, m_History, m_Assets, FileSearcher::search(m_History.getCWD(), searchDirective));
		m_FileView.swap(m_RestoreFileView);
	}

	void FileManager::destroySearch() {
		m_FileView.swap(m_RestoreFileView);
		m_RestoreFileView.reset();
	}

	bool FileManager::hasSearch() {
		return m_RestoreFileView.get();
	}

	void FileManager::onUpdate() {
	}

	void C78E::FileManager::onImGuiRender() {
		ImGui::Begin(m_Title.c_str());
		
		switch (m_UIView) {
		case C78E::FileManager::ShowFiles: showFileView(); break;
		case C78E::FileManager::OpenElement: showOpenElement(); break;
		case C78E::FileManager::SaveElement: showSaveElement(); break;
		default: ImGui::Text("FileManager::onImGuiRender: Broken View!"); break;
		}

		ImGui::End();
	}

	
	void FileManager::showFileView() {
		m_NavBar.show(0.7f);
		Gui::SameLine();
		m_SearchBar.show(1.f);

		if (ImGui::BeginTable("FileManagerSplitView", 2, ImGuiTableFlags_Resizable)) {
			ImGui::TableNextColumn();

			m_PoIPanel.show();

			ImGui::TableNextColumn();

			m_FileView->show();

			ImGui::EndTable();
		}
	}

	void FileManager::showOpenElement() {
		showFileView();
		C78_CORE_ASSERT(m_OpenFileBar, "FileManager::showOpenElement: missing OpenFileBar!");
		m_OpenFileBar->show();
	}

	void FileManager::showSaveElement() {
		showFileView();
		C78_CORE_ASSERT(m_SaveFileBar, "FileManager::showOpenElement: missing SaveFileBar!");
		m_SaveFileBar->show();
	}

}
