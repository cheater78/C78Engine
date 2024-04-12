#pragma once

namespace C78E {

	class FileSearchBar {

	public:
	};

	class FileNavBar {



	};

	class FilePoIPanel {

	};

	class FileView { //abstract
	public:
		FileView(FilePath cwd);
		FileView(const FileView& other) = delete;
		~FileView();

		virtual void onUpdate(FilePath cwd) = 0;
		virtual void show() = 0;

	private:
		FilePath m_CWD;
	};

	class FileViewGrid {

	};

	class FileViewList {

	};

}
