#include "ProjectManager.h"

namespace C78Editor {

	C78E::FilePath ProjectManager::s_CurrentProjectFile = "";
	C78E::Ref<C78E::FileManager> ProjectManager::s_FileManager = nullptr;
	std::string ProjectManager::s_FileManagerOwner = "";
	C78E::FilePath ProjectManager::s_FileManagerResult = "";

	ProjectManager::UIElement ProjectManager::s_UIElement = ProjectManager::START;


	void ProjectManager::init() {
		s_CurrentProjectFile = "";
	}

	void ProjectManager::onUpdate() {
		s_FileManager->onUpdate();
	}

	void ProjectManager::onImGuiMainMenuBar() {

	}

	bool ProjectManager::onImGuiRender() {
		if (isProjectOpen()) return false;

		glm::vec2 fullWindowSize = { C78E::Application::get().getWindow().getWidth(), C78E::Application::get().getWindow().getHeight() };
		glm::vec2 windowSize = fullWindowSize / 1.6f;
		glm::vec2 windowPos = (fullWindowSize - windowSize) / 2.f;

		ImGui::SetNextWindowPos({ windowPos.x, windowPos.y });
		ImGui::SetNextWindowSize({ windowSize.x, windowSize.y }); // Maximized
		if (!ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) return false;

		
		switch (s_UIElement) {
		case C78Editor::ProjectManager::START: {
			if (ImGui::Button("Create")) {
				s_UIElement = ProjectManager::CREATE;
			}
			if (ImGui::Button("Open")) {
				s_UIElement = ProjectManager::OPEN;
			}
			} break;
		case C78Editor::ProjectManager::CREATE:
			if (ImGui::Button("Close")) s_UIElement = ProjectManager::START;
			showProjectCreate();
			break;
		case C78Editor::ProjectManager::OPEN:
			if (ImGui::Button("Close")) s_UIElement = ProjectManager::START;
			showProjectOpen();
			break;
		default: break;
		}
		
		ImGui::End();

		if(s_FileManager)
			s_FileManager->onImGuiRender();

		return true;
	}

	void ProjectManager::onProjectOpen() {
		if (isProjectOpen()) onProjectClose();

		s_CurrentProjectFile = C78E::FileDialogs::openFile(C78_FILE_EXT_PROJECT);
		C78E::Project::load(s_CurrentProjectFile);
	}

	void ProjectManager::onProjectSave() {
		if (s_CurrentProjectFile.empty() || !std::filesystem::exists(s_CurrentProjectFile.parent_path())) {
			//s_CurrentProjectFile = C78E::FileDialogs::saveFile(C78_FILE_EXT_PROJECT);
			C78_EDITOR_ASSERT(false, "Trying to Save Project but loaction unknown!");
			// TODO: FileManager SaveDialog
			if (s_CurrentProjectFile.empty() || !std::filesystem::exists(s_CurrentProjectFile.parent_path())) return;
		}

		if (s_CurrentProjectFile.extension().empty())
			s_CurrentProjectFile += C78_FILE_EXT_PROJECT;

		C78E::Project::saveActive(s_CurrentProjectFile);
	}

	void ProjectManager::onProjectClose() {
		C78E::Project::close();
		s_CurrentProjectFile = "";
	}
	

	void ProjectManager::showProjectCreate() {

		//ProjectName
		static C78E::Gui::TextInput projectNameTI("ProjectName", "Untitled Project");
		projectNameTI.show();
		//~ProjectName~



		//ProjectDirectory
		static C78E::Gui::TextInput projectDirectoryTI("ProjectDirectory", getDefaultProjectPath().string());
		static C78E::Gui::TextButton projectDirectoryTB("...",
			[](void) -> void {
				s_FileManager = C78E::createRef<C78E::FileManager>("");
				s_FileManager->setOpenDialog(C78E::FileSystem::EntryType::DIRECTORY);
				s_FileManagerOwner = "ProjectDirectory";
			}
		);

		if (s_FileManagerOwner == "ProjectDirectory" && s_FileManager->dialogReady()) {
			projectDirectoryTI.setContent(s_FileManager->getDialogResult().string());
			s_FileManager = nullptr;
			s_FileManagerOwner = "";
		}

		projectDirectoryTI.show();
		C78E::Gui::SameLine();
		projectDirectoryTB.show();
		//~ProjectDirectory~
		


		//AssetDirectory
		static C78E::Gui::TextInput assetDirectoryTI("AssetDirectory", getDefaultProjectPath().string());
		static C78E::Gui::TextButton assetDirectoryTB("...",
			[](void) -> void {
				s_FileManager = C78E::createRef<C78E::FileManager>("");
				s_FileManager->setOpenDialog(C78E::FileSystem::EntryType::DIRECTORY);
				s_FileManagerOwner = "AssetDirectory";
			}
		);

		if (s_FileManagerOwner == "AssetDirectory" && s_FileManager->dialogReady()) {
			assetDirectoryTI.setContent(s_FileManager->getDialogResult().string());
			s_FileManager = nullptr;
			s_FileManagerOwner = "";
		}

		assetDirectoryTI.show();
		C78E::Gui::SameLine();
		assetDirectoryTB.show();
		//~AssetDirectory~



		//AssetRegistry
		static C78E::Gui::TextInput assetRegistryTI("AssetRegistry", getDefaultProjectPath().string());
		static C78E::Gui::TextButton assetRegistryTB("...",
			[](void) -> void {
				s_FileManager = C78E::createRef<C78E::FileManager>("");
				s_FileManager->setSaveDialog(".ace", "AssetRegistry");
				s_FileManagerOwner = "AssetRegistry";
			}
		);

		if (s_FileManagerOwner == "AssetRegistry" && s_FileManager->dialogReady()) {
			assetRegistryTI.setContent(s_FileManager->getDialogResult().string());
			s_FileManager = nullptr;
			s_FileManagerOwner = "";
		}

		assetRegistryTI.show();
		C78E::Gui::SameLine();
		assetRegistryTB.show();
		//~AssetRegistry~
		

		//TODO: Script Path
		

		//CreateProject
		if (ImGui::Button("Create")) {
			C78E::Project::create( {
					projectNameTI.getContent(),
					0, //TODO: Create Scene
					assetDirectoryTI.getContent(),
					assetRegistryTI.getContent(), // Relative to AssetDirectory
					"./PlaceHolder",
				}
			);
			s_CurrentProjectFile = (C78E::FilePath)projectDirectoryTI.getContent() / (C78E::FilePath)(projectNameTI.getContent() + C78_FILE_EXT_PROJECT);
			if (std::filesystem::exists(s_CurrentProjectFile)) {
				//TODO: ask for override or open
				C78_EDITOR_WARN("Trying to create new Project: {}, already exists, overriding...");
			}
			C78E::Project::saveActive(s_CurrentProjectFile);
			s_UIElement = ProjectManager::START;
		}
		//~CreateProject~
	}

	void ProjectManager::showProjectOpen() {

		if (s_FileManagerOwner != "ProjectOpen") {
			s_FileManager = C78E::createRef<C78E::FileManager>("");
			s_FileManager->setOpenDialog(C78E::FileSystem::EntryType::PROJECT);
			s_FileManagerOwner = "ProjectOpen";
		}

		if (s_FileManagerOwner == "ProjectOpen" && s_FileManager->dialogReady()) {
			s_CurrentProjectFile = s_FileManager->getDialogResult();
			C78E::Project::load(s_CurrentProjectFile);
			s_UIElement = ProjectManager::START;
		}

	}

}
