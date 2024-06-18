#pragma once
#include <C78E/Core/Timestep.h>

#include <C78E/Core/FileSystem/FileHistory.h>
#include <C78E/Core/FileSystem/FileSearcher.h>

#include "FileManagerAssets.h"
#include "FileManagerUI.h"

namespace C78E::GUI {

	class FileManager : public Visible {
	public:
		static Ref<FileManager> createFileManager(const FilePath& baseDirectoryPath);
		static Ref<FileManager> createOpenDialog(
			const FilePath& defaultFilePath,
			FileSystem::EntryType type,
			const std::string& widgetTitle = ""
		);
		static Ref<FileManager> createSaveDialog(
			const FilePath& defaultFilePath,
			const std::string& extension,
			const std::string& defaultName = "Untitled",
			const std::string& widgetDirectoryTitle = "",
			bool dedicatedFileNameInput = false,
			const std::string& widgetFileNameTitle = ""
		);

	public: //TODO: should be private, xutil needs default
		FileManager() = default;
	public:
		FileManager(const FileManager& other) = delete;
		~FileManager() = default;
		
		void onUpdate(Timestep dt);
		void onImGuiRender();

		void widget();

		bool dialogReady() const;
		FilePath getDialogResult();

		void createSearch(const std::string& searchDirective);
		void destroySearch();
		bool hasSearch();

		void setTitle(const std::string& title);

		FileManagerUIWindow::Type getType() const;

	private:
		Ref<FileHistory> m_History; //TODO: check if order matters
		Ref<FileManagerAssets> m_Assets;
		
		Ref<FileManagerUIWindow> m_UIWindow;

		friend class FileManagerUIElement;
		friend class FileManagerUIWindow;
	};

}
