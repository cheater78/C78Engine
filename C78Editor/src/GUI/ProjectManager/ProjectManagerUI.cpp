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

			if(m_PCCB)
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
		if (auto projectManager = m_ProjectManager.lock()) {

			imGuiPlaceNextWinmain();
			ImGui::Begin("Create Project");
			
			if (!ImGui::IsWindowFocused()) {
				abortCreateProcess();
				ImGui::End();
				return;
			}


			m_PCCB->projectName.show();
			m_PCCB->projectDirectory.show();
			ImGui::SameLine();
			m_PCCB->saveProjectFile.show();

			m_PCCB->assetRegistry.show();
			m_PCCB->assetDirectory.show();
			ImGui::SameLine();
			m_PCCB->saveAssetRegistryFile.show();
			
			
			m_PCCB->scriptModulePath.show();

			if (ImGui::Button("Cancel")) {
				abortCreateProcess();
				ImGui::End();
				return;
			}
			ImGui::SameLine();
			if (ImGui::Button("Create Project")) {
				C78E::ProjectConfig cfg{
					m_PCCB->getProjectName(),
					C78E::AssetHandle::invalid(),
					m_PCCB->getAssetDirectoryPath(),
					m_PCCB->getAssetRegistryFilePath(),
					m_PCCB->getScriptModulePath(),
				};

				projectManager->createProject(cfg);
				projectManager->saveProject(m_PCCB->getProjectFilePath());

				ProjectHistory::projects.insert(m_PCCB->getProjectFilePath());
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
				if (ImGui::Button("Save"))
					projectManager->saveProject();
				ImGui::SameLine();
				if (ImGui::Button("Reload"))
					projectManager->reloadProject();
				ImGui::SameLine();
				if (ImGui::Button("Close"))
					projectManager->closeProject();

				if (projectManager->hasActiveProject()) {
					C78E::ProjectConfig projConf = projectManager->getActiveProject()->getConfig();

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
			C78_ERROR("ProjectManagerUI::drawProjectInfo: Called without m_ProjectManager!");
			return;
		}
	}

	void ProjectManagerUI::startCreateProcess() {
		m_PCCB = C78E::createRef<ProjectCreateConfigBuffers>();
	}

	void ProjectManagerUI::abortCreateProcess() {
		m_PCCB = nullptr;
	}

}
