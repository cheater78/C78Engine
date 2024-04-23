#pragma once

#include "core/FileHistory.h"
#include "core/FileAssets.h"
#include "core/FileViewElements.h"
#include "core/FileSearcher.h"

namespace C78E {

	class FileManager {
	public:
		enum View {
			ShowFiles,
			OpenElement,
			SaveElement
		};
	public:
		FileManager(FilePath defaultDirectory);
		FileManager(const FileManager& other) = delete;
		~FileManager() = default;

		void setOpenDialog(FileSystem::EntryType type);
		void setSaveDialog(const std::string& extension, const std::string& defaultName = "Untitled");

		bool dialogReady() const;
		FilePath getDialogResult();

		void createSearch(const std::string& searchDirective);
		void destroySearch();
		bool hasSearch();
		
	public: //GUI
		void setView(View view) { m_UIView = view; }
		void setTitle(std::string title) { m_Title = title; }

		void onUpdate();
		void onImGuiRender();

	private:
		void showFileView();
		void showOpenElement();
		void showSaveElement();
		//~GUI



	private:
		std::string m_Title = "File Manager";
		UISettings m_UISettings;

		// ORDER MATTERS !!!
		FileHistory m_History;
		FileAssets m_Assets;

		FileNavBar m_NavBar;
		FileSearchBar m_SearchBar;
		FilePoIPanel m_PoIPanel;
		Scope<FileView> m_FileView;
		Scope<FileView> m_RestoreFileView = nullptr;
		
		Scope<OpenFileBar> m_OpenFileBar = nullptr;
		Scope<SaveFileBar> m_SaveFileBar = nullptr;
		

	private:
		View m_UIView;
	};

}
