#include "ProjectManagerUI.h"

#include "../../C78Editor/src/Config/ProjectHistory.h"

namespace C78Editor::GUI {

	static void imGuiPlaceNextWinmain() {
		const auto& win = C78E::Application::get().getWindow();
		const glm::vec2 winSize{ win.getWidth(), win.getHeight() };
		const glm::vec2 panelSize = winSize / glm::golden_ratio<float>();

		ImGui::SetNextWindowPos(C78E::GUI::toImVec((winSize - panelSize) / 2.f));
		ImGui::SetNextWindowSize(C78E::GUI::toImVec(panelSize));
	}


	ProjectManagerUI::ProjectManagerUI(C78E::Ref<C78E::ProjectManager> projectManager)
		: m_ProjectManager{ projectManager } {
		ProjectHistory::load();
	}

	ProjectManagerUI::~ProjectManagerUI() { ProjectHistory::save(); }

	void ProjectManagerUI::onImGuiMainMenuBar() {
	}

	void ProjectManagerUI::onImGuiRender() {
		if (auto projectManager = m_ProjectManager.lock()) {
			
			drawProjectInfo();

			if(m_ProjectCreateInProgress)
				drawCreateProject();

			if (!projectManager->hasActiveProject())
				drawNoProject();
		}
	}

	void ProjectManagerUI::drawNoProject() {
		if(auto projectManager = m_ProjectManager.lock()) {

			imGuiPlaceNextWinmain();
			ImGui::Begin("Project Manager");

			

			if (ImGui::Button("Create Project")) {
				startCreateProcess();
			}
			if (ImGui::Button("Open Project")) {
				C78E::FilePath path = C78E::FileDialogs::openFile({C78E::FileSystem::EntryType::Project}, C78E::FileSystem::C78RootDirectory, "", "C78E Open Project");
				if (!path.empty()) {
					projectManager->openProject(path);
					ProjectHistory::projects.insert(path);
					ProjectHistory::save();
				}
			}

			if (ProjectHistory::projects.size()) {
				for (auto& path : ProjectHistory::projects) {
					if (ImGui::Button(path.string().c_str())) {
						projectManager->openProject(path);
					}
				}
			}
			else {
				ImGui::Text("No Projects opened recently!");
			}

			ImGui::End();
		}
		else {
			C78E_ERROR("ProjectManagerUI::drawNoProject: Called without m_ProjectManager!");
			return;
		}
		
	}

	void ProjectManagerUI::drawCreateProject() {
		if (auto projectManager = m_ProjectManager.lock()) {

			imGuiPlaceNextWinmain();
			ImGui::Begin("Create Project");
			
			if (!ImGui::IsWindowFocused()) {
				abortCreateProcess();
				ImGui::End();
				return;
			}

			C78E::GUI::drawLabeledTextInput<C78E_PROJECT_NAME_MAX_LENGTH>("ProjectName:", m_ProjectCreateConfig.name);
			C78E::GUI::drawLabeledFileInput<PATH_MAX>("ProjectDirectory", m_ProjectCreateDirectory, false, {C78E::FileSystem::EntryType::Directory});
			C78E::GUI::drawLabeledFileInput<PATH_MAX>("AssetDirectory", m_ProjectCreateConfig.assetDirectory, false, {C78E::FileSystem::EntryType::Directory});
			C78E::GUI::drawLabeledFileInput<PATH_MAX>("AssetRegistry", m_ProjectCreateConfig.assetRegistryPath, true, {C78E::FileSystem::EntryType::AssetRegistry});
			C78E::GUI::drawLabeledFileInput<PATH_MAX>("ScriptModule", m_ProjectCreateConfig.scriptModulePath, true, {C78E::FileSystem::EntryType::Binary});

			if (C78E::GUI::drawTextButton("Cancel")) {
				abortCreateProcess();
				ImGui::End();
				return;
			}
			ImGui::SameLine();
			if (C78E::GUI::drawTextButton("Create Project")) {
				m_ProjectCreateConfig.normalize(m_ProjectCreateDirectory);
				projectManager->createProject(m_ProjectCreateDirectory, m_ProjectCreateConfig);
				projectManager->saveProject(m_ProjectCreateConfig.getProjectFilePath(m_ProjectCreateDirectory));

				ProjectHistory::projects.insert(m_ProjectCreateConfig.getProjectFilePath(m_ProjectCreateDirectory));
				ProjectHistory::save();
			}
			ImGui::End();
		}
		else {
			C78E_ERROR("ProjectManagerUI::drawCreateProject: Called without m_ProjectManager!");
			return;
		}
	}

	void ProjectManagerUI::drawProjectInfo() {
		if (auto projectManager = m_ProjectManager.lock()) {

			ImGui::Begin("Project Info");

			if (projectManager->hasActiveProject()) {
				if (ImGui::Button("Save"))
					projectManager->saveProject();
				ImGui::SameLine();
				if (ImGui::Button("Reload"))
					projectManager->reloadProject();
				ImGui::SameLine();
				if (ImGui::Button("Close"))
					projectManager->closeProject();

				if (projectManager->hasActiveProject()) {
					C78E::Project::Config projConf = projectManager->getActiveProject()->getConfig();

					ImGui::Text(("Project: " + projConf.name).c_str());
					ImGui::Text((std::string("File: ") + ((projectManager->hasActiveProjectFile()) ? projectManager->getActiveProjectFile().string() : "No File set!")).c_str());
					ImGui::Text(("StartScene: " + std::to_string(projConf.startScene)).c_str());
					ImGui::Text(("AssetDirectory: " + projConf.assetDirectory.string()).c_str());
					ImGui::Text(("AssetRegistry: " + projConf.assetRegistryPath.string()).c_str());
					ImGui::Text(("ScriptModules: " + projConf.scriptModulePath.string()).c_str());

				}
				else {
					ImGui::Text("No Active Project!");
				}
			}
			else {
				ImGui::Text("No Active Project!");
			}
			ImGui::End();
		}
		else {
			C78E_ERROR("ProjectManagerUI::drawProjectInfo: Called without m_ProjectManager!");
			return;
		}
	}

	void ProjectManagerUI::startCreateProcess() {
		m_ProjectCreateInProgress = true;
	}

	void ProjectManagerUI::abortCreateProcess() {
		m_ProjectCreateInProgress = false;
		m_ProjectCreateDirectory = C78E::FileSystem::C78RootDirectory;
		m_ProjectCreateConfig = {};
	}

}
