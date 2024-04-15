#pragma once

#include "core/FileHistory.h"
#include "core/FileAssets.h"
#include "core/FileViewElements.h"

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

		Ref<FileView> m_FileView;
		FileSearchBar m_SearchBar;
		FileNavBar m_NavBar;
		


	private:
		View m_UIView;
	};

}
