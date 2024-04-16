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
		

	private:
		View m_UIView;
	};

}
