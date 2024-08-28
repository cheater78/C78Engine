#include "ProjectManagerUI.h"

#include "../../C78Editor/src/Config/ProjectHistory.h"

namespace C78Editor::GUI {

	ProjectManagerUI::ProjectManagerUI(C78E::Ref<C78E::ProjectManager> projectManager)
		: m_ProjectManager{ projectManager }, m_ProjectCreateConfig{} {
		ProjectHistory::load();
	}

	ProjectManagerUI::~ProjectManagerUI() { ProjectHistory::save(); }

	void ProjectManagerUI::onImGuiMainMenuBar() {
	}

	void ProjectManagerUI::onImGuiRender() {
		if (auto projectManager = m_ProjectManager.lock()) {
			
			drawNoProject();
			drawProjectInfo();
			drawCreateProject();
		}
	}

	void ProjectManagerUI::drawNoProject() {
		if(auto projectManager = m_ProjectManager.lock()) {

			ImGui::Begin("Project Manager");

			if (ImGui::Button("Create Project")) {
				m_ProjectCreateStage = 1;
			}
			if (ImGui::Button("Open Project")) {
				C78E::FilePath path = C78E::FileDialogs::openFile("C78Project (*.pce)\0*.pce\0");
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
			C78_ERROR("ProjectManagerUI::drawNoProject: Called without m_ProjectManager!");
			return;
		}
		
	}

	void ProjectManagerUI::drawCreateProject() {
		if (!m_ProjectCreateStage) return;
		if (auto projectManager = m_ProjectManager.lock()) {

			ImGui::Begin("Create Project");
			// buffer to complex for UI -> move to projectmanager
			ImGui::InputText("Project Name", m_ProjectCreateConfig.name.data(), m_ProjectCreateConfig.name.size());
			ImGui::InputText("Project Directory", m_ProjectCreateDirectory.string().data(), m_ProjectCreateDirectory.string().size());
			ImGui::InputText("Asset Directory", m_ProjectCreateConfig.assetDirectory.string().data(), m_ProjectCreateConfig.assetDirectory.string().size());
			ImGui::InputText("Asset Registry", m_ProjectCreateConfig.assetRegistryPath.string().data(), m_ProjectCreateConfig.assetRegistryPath.string().size());
			ImGui::InputText("ScriptModulePath", m_ProjectCreateConfig.scriptModulePath.string().data(), m_ProjectCreateConfig.scriptModulePath.string().size());

			if (ImGui::Button("Create Project")) {
				C78E::FilePath projectFile = m_ProjectCreateDirectory / C78E::FilePath(m_ProjectCreateConfig.name + std::string(C78E_FILE_EXT_PROJECT));
				projectManager->createProject(m_ProjectCreateConfig);
				projectManager->saveProject(projectFile);

				ProjectHistory::projects.insert(projectFile);
				ProjectHistory::save();
			}

			ImGui::End();
		}
		else {
			C78_ERROR("ProjectManagerUI::drawCreateProject: Called without m_ProjectManager!");
			return;
		}
	}

	void ProjectManagerUI::drawProjectInfo() {
		if (auto projectManager = m_ProjectManager.lock()) {

			ImGui::Begin("Project Info");

			if (projectManager->hasActiveProject()) {
				C78E::ProjectConfig projConf = projectManager->getActiveProject()->getConfig();

				ImGui::Text(("Project: " + projConf.name).c_str());
				ImGui::Text(("File: " + (projectManager->hasActiveProjectFile()) ? projectManager->getActiveProjectFile().string() : "No File set!").c_str());
				ImGui::Text(("StartScene: " + std::to_string(projConf.startScene)).c_str());
				ImGui::Text(("AssetDirectory: " + projConf.assetDirectory.string()).c_str());
				ImGui::Text(("AssetRegistry: " + projConf.assetRegistryPath.string()).c_str());
				ImGui::Text(("ScriptModules: " + projConf.scriptModulePath.string()).c_str());

				if (ImGui::Button("Reload")) {
					projectManager->reloadProject();
				}
				ImGui::SameLine();
				if (ImGui::Button("Save")) {
					projectManager->saveProject();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close")) {
					projectManager->closeProject();
				}


			}
			else {
				ImGui::Text("No Project open...");
			}
			ImGui::End();
		}
		else {
			C78_ERROR("ProjectManagerUI::drawProjectInfo: Called without m_ProjectManager!");
			return;
		}
	}

}
