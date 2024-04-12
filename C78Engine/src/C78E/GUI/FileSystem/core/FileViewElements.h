#pragma once
#include <C78E/GUI/Elements/ImGuiUtils.h>

#include "FileHistory.h"
#include "FileAssets.h"
#include "FileViewElements.h"


namespace C78E {

	struct UISettings {
		uint32_t screenWidth = 2560; //TODO: like just dont
		uint32_t screenHeight = 1440;
		float scale = 1.0f;
	};

	class FileSearchBar {
	public:
	private:
	};

	class FileNavBar {
	public:
		FileNavBar(FileHistory& history, FileAssets& assets);
		FileNavBar(const FileNavBar& other) = delete;
		~FileNavBar();

		void show();
	private:
		FileHistory& m_History;
		FileAssets& m_Assets;
		UISettings m_UISettings;
		Gui::ImageButton m_BackButton;
		Gui::ImageButton m_ForwardButton;
		Gui::ImageButton m_HomeButton;
		Gui::ImageButton m_ParentButton;
		Gui::TextInput m_PathInput;
	};

	class FilePoIPanel {

	};

	class FileView { //abstract
	public:
		using SortFilter = std::vector<FileSystem::EntryType>;
		static const SortFilter SortNoFilter;

		enum FileViewType {
			Grid,
			List
		};
	public:
		FileView() = delete;
		FileView(FileHistory& history, FileAssets& assets);
		FileView(const FileView& other) = delete;
		~FileView();

		virtual void show() = 0;
		virtual FileViewType getType() = 0;
	public:
		static void onFileClick(FilePath file);
	protected:
		FileHistory& m_History;
		FileAssets& m_Assets;
		SortFilter m_Filter;
		UISettings m_UISettings;
	};

	class FileViewGrid : public FileView {
	public:
		FileViewGrid() = delete;
		FileViewGrid(FileHistory& history, FileAssets& assets);
		FileViewGrid(const FileViewGrid& other) = delete;
		~FileViewGrid();

		virtual void show() override;
		virtual FileViewType getType() { return Grid; }
	private:
		std::unordered_map<FilePath, Gui::ImageButton> m_FileCards;
	};

	class FileViewList : public FileView {
	public:
		FileViewList() = delete;
		FileViewList(FileHistory& history, FileAssets& assets);
		FileViewList(const FileViewList& other) = delete;
		~FileViewList();

		virtual void show() override;
		virtual FileViewType getType() { return List; }
	private:
		std::unordered_map<FilePath, Gui::ImageButton> m_FileCards; //TODO: rename
	};

}
