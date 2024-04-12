#pragma once

#include "core/FileHistory.h"
#include "core/FileAssets.h"
#include "core/FileViewElements.h"

namespace C78E {

	class FileManager {
	public:
		using SortFilter = std::vector<FileSystem::EntryType>;
	public:
		enum View {
			FileView,
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

		void showTopBar();
		void showFileGrid();

		bool showSingleFileEntry(std::string label, C78E::Ref<C78E::Texture2D> icon, uint32_t size);

		glm::vec2 getContentRegionAvail();
		//~GUI



	private:
		std::string m_Title = "File Manager";

		FileHistory m_History;
		FileAssets m_Assets;
		Ref<FileView> m_FileView;
		SortFilter m_SortFilter;


	private:
		struct UISettings {
			uint32_t topBarElementHeight = 20;
			uint32_t elementCount = 11;
			std::string assetDirectory = "../C78Editor/asset/texture/FileManager"; //TODO: no hardcode duh
		} m_UISettings;
		View m_UIView;
	};

}
