#pragma once

#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class ProjectManager {
	public:

		static void init() {
			s_CurrentProjectFile = "";
		}

		static void onUpdate() {
		}

		static void onImGuiMainMenuBar() {

		}

		static bool onImGuiRender() {
			if (isProjectOpen()) return false;
			
			glm::vec2 fullWindowSize = { C78E::Application::get().getWindow().getWidth(), C78E::Application::get().getWindow().getHeight() };
			glm::vec2 windowSize = fullWindowSize / 1.6f;
			glm::vec2 windowPos = (fullWindowSize - windowSize) / 2.f;

			ImGui::SetNextWindowPos({ windowPos.x, windowPos.y });
			ImGui::SetNextWindowSize({windowSize.x, windowSize.y}); // Maximized
			if (!ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) return false;
			

			if (ImGui::Button("Create")) onProjectCreate();
			if (ImGui::Button("Open")) onProjectOpen();

			ImGui::End();
			return true;
		}

		static void onProjectCreate() {
			C78E::Project::create();

			C78E::ProjectConfig& config = C78E::Project::getActive()->getConfig();

			config.name = "C78Project";
			config.assetDirectory = "./C78Project/assets";
			config.assetRegistryPath = "./C78Project/assets/AssetRegistry.yml";
			config.scriptModulePath = "./C78Project/assets/scripts";
			config.startScene = 0;

			onProjectSave();
		}

		static void onProjectOpen() {
			if (isProjectOpen()) onProjectClose();

			s_CurrentProjectFile = C78E::FileDialogs::openFile(C78_FILE_EXT_PROJECT);
			C78E::Project::load(s_CurrentProjectFile);
		}

		static void onProjectSave() {
			if (s_CurrentProjectFile.empty() || !std::filesystem::exists(s_CurrentProjectFile.parent_path()) ) {
				s_CurrentProjectFile = C78E::FileDialogs::saveFile(C78_FILE_EXT_PROJECT);
				if (s_CurrentProjectFile.empty() || !std::filesystem::exists(s_CurrentProjectFile.parent_path())) return;
			}

			if (s_CurrentProjectFile.extension().empty())
				s_CurrentProjectFile += C78_FILE_EXT_PROJECT;

			C78E::Project::saveActive(s_CurrentProjectFile);
		}

		static void onProjectClose() {
			C78E::Project::close();
			s_CurrentProjectFile = "";
		}

		// Shortcuts
		static bool onKeyPressedEvent(C78E::KeyPressedEvent event) {
			if (!isProjectOpen()) return false;

			if (event.getKeyCode() == C78E::Key::S && C78E::Input::isKeyPressed(C78E::Key::LeftControl)) {
				onProjectSave();
				return true;
			}
			
			return false;
		}


	private:
		static bool isProjectOpen() { return (bool)C78E::Project::getActive(); }

	private:

		static C78E::FilePath s_CurrentProjectFile;

	};

}


