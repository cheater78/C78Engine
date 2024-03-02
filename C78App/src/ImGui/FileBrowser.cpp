#include "FileBrowser.h"
#include "imgui/imgui.h"

namespace C78Editor {
	//UI
	uint32_t FileBrowser::s_ColumnElements = 11;

	static const std::filesystem::path c_FolderTextureFile = std::filesystem::relative("assets/textures/Folder.png");
	static const std::filesystem::path c_FileTextureFile = std::filesystem::relative("assets/textures/File.png");
	static const std::filesystem::path c_FileUnknownTextureFile = std::filesystem::relative("assets/textures/FileUnknown.png");

	C78E::Ref<C78E::Texture2D> FileBrowser::s_FolderTexture;
	C78E::Ref<C78E::Texture2D> FileBrowser::s_FileTexture;
	C78E::Ref<C78E::Texture2D> FileBrowser::s_FileUnknownTexture;

	//Logic
	std::filesystem::path FileBrowser::s_CWD;
	std::filesystem::path FileBrowser::s_CurrentProjectPath;
	std::filesystem::path FileBrowser::s_LastCWD;

	FileBrowser::DirectoryStructure FileBrowser::s_CWDStructure;

	

	void FileBrowser::init(std::filesystem::path currentProjectPath) {
		s_CWD = currentProjectPath;
		s_CurrentProjectPath = currentProjectPath;
		s_LastCWD = currentProjectPath;

		s_CWDStructure.sortFilter = { DIRECTORY, PROJECT, SCENE, IMAGE, SHADER, MODEL, MATERIAL, FONT, MISC };
		updateCurrentDirectoryStructure();

		s_FolderTexture = C78E::Texture2D::create(c_FolderTextureFile.string());
		s_FileTexture = C78E::Texture2D::create(c_FileTextureFile.string());
		s_FileUnknownTexture = C78E::Texture2D::create(c_FileUnknownTextureFile.string());

	}

	void FileBrowser::onUpdate() { }

	void FileBrowser::onImGuiRender() {
		ImGui::Begin("FileBrowser");

		ImVec2 size = ImGui::GetContentRegionAvail();
		uint32_t x = static_cast<uint32_t>(size.x);
		s_CWDStructure.elementSize = (x / s_ColumnElements)*0.90f;

		showCWDNavigator();
		
		showFileNavigator();
		
		ImGui::End();
	}

	FileBrowser::EntryType FileBrowser::getEntryType(std::filesystem::path filepath) {
		if (std::filesystem::is_directory(filepath))
			return EntryType::DIRECTORY;
		std::string ext = filepath.extension().string();
		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
			return EntryType::IMAGE;
		else if (ext == ".obj")
			return EntryType::MODEL;
		else if (ext == ".mtl")
			return EntryType::MATERIAL;
		else if (ext == ".glsl")
			return EntryType::SHADER;
		else if (ext == ".ttf")
			return EntryType::FONT;
		else if (ext == ".pce")
			return EntryType::PROJECT;
		else if (ext == ".sce")
			return EntryType::SCENE;

		return EntryType::MISC;
	}

	void FileBrowser::showCWDNavigator() {
		// Back 
		if (ImGui::Button("<") && s_CWD.has_parent_path()) {
			changeDirectory(s_CWD.parent_path());
		}
		
#ifdef C78_PLATFORM_WINDOWS
		else {
			//TODO: Drive Selector
		}
#endif //C78_PLATFORM_WINDOWS

		ImGui::SameLine();

		if (ImGui::Button(s_CurrentProjectPath.filename().string().c_str())) {
			changeDirectory(s_CurrentProjectPath);
		}

		ImGui::SameLine();

		if (ImGui::Button(">") && s_CWD.has_parent_path()) {
			changeDirectory(s_LastCWD);
		}

		ImGui::SameLine();

		ImGui::Text(s_CWD.string().c_str()); //TODO: editable

		ImGui::SameLine();

		if (ImGui::Button("+")) {
			s_ColumnElements--;
		}

		ImGui::SameLine();

		if (ImGui::Button("-")) {
			s_ColumnElements++;
		}

	}


	void FileBrowser::showFileNavigator() {
		if (ImGui::BeginTable("Files", s_ColumnElements, ImGuiTableFlags_None)) {

			if (s_CWD != s_CWDStructure.path)
				updateCurrentDirectoryStructure();

			for(EntryType type : s_CWDStructure.sortFilter)
				for (auto& kv : s_CWDStructure.entries) {
					if (kv.second != type) continue;

					ImGui::TableNextColumn();

					showSingleFileCard(kv.first, type);

				}

			ImGui::EndTable();
		}
	}

	void C78Editor::FileBrowser::showSingleFileCard(std::filesystem::path filepath, EntryType type) {
		std::string fileName = filepath.filename().string();

		switch (type) {
		case C78Editor::FileBrowser::DIRECTORY:

			if (showSingleFileEntry(fileName, s_FolderTexture))
				changeDirectory(filepath);

			break;
		case C78Editor::FileBrowser::IMAGE:

			if (showSingleFileEntry(fileName, getIcon(filepath))) {
				// IMAGE click
			}

			break;
		case C78Editor::FileBrowser::SHADER:

			if (showSingleFileEntry(fileName, s_FileTexture)) {
				// SHADER click
			}

			break;
		case C78Editor::FileBrowser::MODEL:

			if (showSingleFileEntry(fileName, s_FileTexture)) {
				// MODEL click
			}

			break;
		case C78Editor::FileBrowser::MATERIAL:

			if (showSingleFileEntry(fileName, s_FileTexture)) {
				// MATERIAL click
			}

			break;
		case C78Editor::FileBrowser::FONT:

			if (showSingleFileEntry(fileName, s_FileTexture)) {
				// FONT click
			}

			break;
		case C78Editor::FileBrowser::PROJECT:

			if (showSingleFileEntry(fileName, s_FileTexture)) {
				// PROJECT click
			}

			break;
		case C78Editor::FileBrowser::SCENE:

			if (showSingleFileEntry(fileName, s_FileTexture)) {
				// SCENE click
			}

			break;
		case C78Editor::FileBrowser::MISC:

			if (showSingleFileEntry(fileName, s_FileUnknownTexture)) {
				// MISC click
			}

			break;
		default: break;
		}

	}

	void FileBrowser::changeDirectory(std::filesystem::path newCWD) {
		if (newCWD != s_CWD)
			s_LastCWD = s_CWD;
		s_CWD = newCWD;
	}

	void FileBrowser::updateCurrentDirectoryStructure() {
		s_CWDStructure.path = s_CWD;
		s_CWDStructure.entries.clear();
		s_CWDStructure.icons.clear();

		//std::vector<std::thread> threads;

		for (const auto& entry : std::filesystem::directory_iterator(s_CWD)) {
			EntryType type = getEntryType(entry);
			s_CWDStructure.entries.emplace(entry, type);

			if (type == EntryType::IMAGE) {
				const std::filesystem::path path = entry.path();
				C78E::RawImage img(path.string().c_str());
				s_CWDStructure.icons.emplace(path, C78E::Texture2D::create(img));
				/*
				threads.push_back(
					std::thread([path]() {
						C78E::RawImage img(path.string().c_str());
						s_CWDStructure.writeIcons.acquire();
						C78_TRACE("Acquired {0}...", path.string());
						s_CWDStructure.icons.emplace(path, C78E::Texture2D::create(img));
						C78_TRACE("Releasing {0}...", path.string());
						s_CWDStructure.writeIcons.release();
					})
				);
				*/
			}
		}
		/*
		for (auto& thread : threads)
			thread.join();
			*/
	}

	bool C78Editor::FileBrowser::showSingleFileEntry(std::string label, C78E::Ref<C78E::Texture2D> icon, uint32_t size) {
		C78E::Ref<C78E::Texture2D> texture = (icon == nullptr) ? s_FileUnknownTexture : icon;
		float x = 1.f;
		float y = 1.f;
		if (texture->getWidth() != texture->getHeight())
			if (texture->getWidth() < texture->getHeight()) x = (float)texture->getWidth() / (float)texture->getHeight();
			else y = (float)texture->getHeight() / (float)texture->getWidth();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		bool clicked = ImGui::ImageButton(label.c_str(), (ImTextureID)texture->getRendererID(), ImVec2((uint32_t)(size*x), (uint32_t)(size*y)));
		ImGui::PopStyleVar();
		ImGui::Text(label.c_str());
		return clicked;
	}
	
	C78E::Ref<C78E::Texture2D> FileBrowser::getIcon(std::filesystem::path file) {
		return (s_CWDStructure.icons.contains(file)) ? s_CWDStructure.icons.at(file) : nullptr;
	}

}