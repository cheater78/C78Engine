#include "C78ePCH.h"
#include "FileManager.h"

#include <C78E/GUI/GUIElements.h>

namespace C78E::GUI {

	Ref<FileManager> FileManager::createFileManager(const FilePath& baseDirectoryPath) {
		Ref<FileManager> fm = createRef<FileManager>();
		fm->m_History = createRef<FileHistory>(baseDirectoryPath);

		fm->m_Assets = createRef<FileManagerAssets>("C:\\dev\\c-cpp\\C78Engine\\C78Editor\\assets\\textures\\FileManager"); // TODO: Editor Asset Handling (Bake in)

		fm->m_UIWindow = createRef<FileManagerUIBrowser>(fm);

		return fm;
	}

	Ref<FileManager> FileManager::createOpenDialog(const FilePath& defaultFilePath, FileSystem::EntryType type, const std::string& widgetTitle) {
		Ref<FileManager> fm = createRef<FileManager>();
		fm->m_History = createRef<FileHistory>(defaultFilePath);

		fm->m_Assets = createRef<FileManagerAssets>("C:\\dev\\c-cpp\\C78Engine\\C78Editor\\assets\\textures\\FileManager"); // TODO: Editor Asset Handling (Bake in)

		auto openUI = createRef<FileManagerUIOpen>(fm, type, defaultFilePath, widgetTitle);
		fm->m_UIWindow = openUI;
		openUI->setResult(defaultFilePath);

		return fm;
	}

	Ref<FileManager> FileManager::createSaveDialog(const FilePath& defaultFilePath, const std::string& extension, const std::string& defaultName, const std::string& widgetDirectoryTitle, bool dedicatedFileNameInput, const std::string& widgetFileNameTitle) {
		Ref<FileManager> fm = createRef<FileManager>();
		fm->m_History = createRef<FileHistory>(defaultFilePath);

		fm->m_Assets = createRef<FileManagerAssets>("C:\\dev\\c-cpp\\C78Engine\\C78Editor\\assets\\textures\\FileManager"); // TODO: Editor Asset Handling (Bake in)

		auto saveUI = createRef<FileManagerUISave>(fm, extension, defaultFilePath, widgetDirectoryTitle, dedicatedFileNameInput, widgetFileNameTitle);
		fm->m_UIWindow = saveUI;
		saveUI->setResult(defaultFilePath / (defaultName + extension));

		return fm;
	}

	void FileManager::onUpdate(Timestep dt) {

	}

	void FileManager::onImGuiRender() {
		if (!m_Visible) return;
		m_UIWindow->show();
	}

	void FileManager::widget() {
		m_UIWindow->widget();
	}

	bool FileManager::dialogReady() const {
		switch (m_UIWindow->getType()) {
		case FileManagerUIWindow::Type::Browser: return false;
		case FileManagerUIWindow::Type::Open: return std::reinterpret_pointer_cast<FileManagerUIOpen>(m_UIWindow)->ready();
		case FileManagerUIWindow::Type::Save: return std::reinterpret_pointer_cast<FileManagerUISave>(m_UIWindow)->ready();
		}
	}

	FilePath FileManager::getDialogResult() {
		switch (m_UIWindow->getType()) {
		case FileManagerUIWindow::Type::Browser: return "";
		case FileManagerUIWindow::Type::Open: return std::reinterpret_pointer_cast<FileManagerUIOpen>(m_UIWindow)->getResult();
		case FileManagerUIWindow::Type::Save: return std::reinterpret_pointer_cast<FileManagerUISave>(m_UIWindow)->getResult();
		}
	}

	void FileManager::createSearch(const std::string& searchDirective) {
		std::thread th(
			[this, searchDirective]() -> void {
				Ref<FileManagerUINavigationPanel> navPanel = std::reinterpret_pointer_cast<FileManagerUINavigationPanel>(m_UIWindow);

				navPanel->createSearch({});

				auto result = FileSearcher::search(m_History->getCWD(), searchDirective);

				navPanel->createSearch(result);
			}
		);
	}

	void FileManager::destroySearch() { std::reinterpret_pointer_cast<FileManagerUINavigationPanel>(m_UIWindow)->destroySearch(); }
	bool FileManager::hasSearch() { return std::reinterpret_pointer_cast<FileManagerUINavigationPanel>(m_UIWindow)->hasSearch(); }

	void FileManager::setTitle(const std::string& title) { m_UIWindow->setTitle(title); }

	FileManagerUIWindow::Type FileManager::getType() const { return m_UIWindow->getType(); }

}
