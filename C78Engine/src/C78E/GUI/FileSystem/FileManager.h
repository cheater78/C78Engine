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

		FileHistory m_History;
		FileAssets m_Assets;

		FileNavBar m_NavBar;
		Ref<FileView> m_FileView;


	private:
		View m_UIView;
	};

}
