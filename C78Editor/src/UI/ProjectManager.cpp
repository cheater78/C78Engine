#include "ProjectManager.h"

namespace C78Editor {

	C78E::FilePath ProjectManager::s_CurrentProjectFile = "";
	C78E::Ref<C78E::FileManager> ProjectManager::s_FileManager = nullptr;

	ProjectManager::UIElement ProjectManager::s_UIElement = ProjectManager::START;


	void ProjectManager::init() {
		s_CurrentProjectFile = "";
		s_FileManager = C78E::createRef<C78E::FileManager>(C78E::FileDialogs::openFile().parent_path());
	}

	void ProjectManager::onUpdate() {

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

		/*
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
		*/
		ImGui::End();

		s_FileManager->onImGuiRender();

		return true;
	}

	void ProjectManager::onProjectCreate(C78E::ProjectConfig config) {
		C78E::Project::create(config);
		onProjectSave();
	}

	void ProjectManager::onProjectOpen() {
		if (isProjectOpen()) onProjectClose();

		s_CurrentProjectFile = C78E::FileDialogs::openFile(C78_FILE_EXT_PROJECT);
		C78E::Project::load(s_CurrentProjectFile);
	}

	void ProjectManager::onProjectSave() {
		if (s_CurrentProjectFile.empty() || !std::filesystem::exists(s_CurrentProjectFile.parent_path())) {
			s_CurrentProjectFile = C78E::FileDialogs::saveFile(C78_FILE_EXT_PROJECT);
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

		C78E::FilePath path = C78E::FileDialogs::openFolder(getDefaultProjectPath(), 0);
		C78_EDITOR_TRACE("Selected Path: {}", path.string());

		static C78E::Gui::TextInput projectNameTI("ProjectName", "Untitled Project");



		projectNameTI.show();


		static C78E::Gui::TextInput assetDirectoryTI("AssetDirectory", getDefaultProjectPath().string());
		static C78E::Gui::TextButton assetDirectoryTB("...",
			[](void) -> void {
				assetDirectoryTI.setContent(C78E::FileDialogs::openFile().string());
			}
		);

		assetDirectoryTI.show();
		C78E::Gui::SameLine();
		assetDirectoryTB.show();



		static C78E::Gui::TextInput assetRegistryTI("AssetRegistry", getDefaultProjectPath().string());
		static C78E::Gui::TextButton assetRegistryTB("...",
			[](void) -> void {
				assetRegistryTI.setContent(C78E::FileDialogs::saveFile(".ace ").string());
			}
		);

		assetRegistryTI.show();
		C78E::Gui::SameLine();
		assetRegistryTB.show();
		

		// SC M Path
		
		if (ImGui::Button("Create")) {

			C78_CORE_ASSERT(false, "");
			onProjectCreate( {
					"Untitled Project",
					0, //TODO: Create Scene
					"assetDirectory",
					"assetRegistryPath", // Relative to AssetDirectory
					"scriptModulePath",
				}
			);
		}
	}

	void ProjectManager::showProjectOpen() {
	}

}
