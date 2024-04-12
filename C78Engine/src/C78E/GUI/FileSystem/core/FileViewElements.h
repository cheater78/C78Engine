#pragma once
#include "FileAssets.h"

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
		struct UISettings {
			uint32_t screenWidth;
			uint32_t screenHeight;
			float scale = 1.0f;
		};
	public:
		FileView(FilePath cwd, FileAssets& assets);
		FileView(const FileView& other) = delete;
		~FileView();

		virtual void onUpdate(FilePath cwd) = 0;
		virtual void show() = 0;

	private:
		FilePath m_CWD;
		FileAssets& m_Assets;
	};

	class FileViewGrid {

	};

	class FileViewList {

	};

}
