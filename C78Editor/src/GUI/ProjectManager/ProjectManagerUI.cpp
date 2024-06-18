#include "ProjectManagerUI.h"

#include "ProjectManager.h"

namespace C78Editor::GUI {



	CreateProjectUI::CreateProjectUI(::C78Editor::GUI::ProjectManager* projectManager)
		: m_ProjectManager(projectManager) {

		
		m_ProjectFileManager = C78E::GUI::FileManager::createSaveDialog(
			m_ProjectManager->getDefaultProjectPath(),
			C78E_FILE_EXT_PROJECT,
			"UntitledProject",
			"Project Directory: ",
			true,
			"ProjectName"
		);
		m_ProjectFileManager->setVisible(false);

		m_AssetFileManager = C78E::GUI::FileManager::createSaveDialog(
			m_ProjectManager->getDefaultProjectPath(),
			C78E_FILE_EXT_ASSETREGISTRY,
			"AssetRegistry",
			"Asset Directory: ",
			true,
			"AssetRegistry File: "
		);
		m_AssetFileManager->setVisible(false);
		
		m_CancelTB = C78E::createRef<C78E::GUI::TextButton>("Cancel",
			[this](void) -> void {
				this->setVisible(false);
				m_ProjectFileManager->setVisible(false);
				m_AssetFileManager->setVisible(false);
			}
		);

		m_CreateProjectTB = C78E::createRef<C78E::GUI::TextButton>("Create Project",
			[this](void) -> void {
				m_ProjectManager->getManager()->createProject({
						m_ProjectFileManager->getDialogResult().filename().string(),
						0, //TODO: Create Scene
						m_AssetFileManager->getDialogResult().parent_path(),
						m_AssetFileManager->getDialogResult().filename(), // Relative to AssetDirectory
						"./PlaceHolder"
					}
				);
				m_ProjectManager->getManager()->saveProject(m_ProjectFileManager->getDialogResult());
				this->setVisible(false);
				m_ProjectFileManager->setVisible(false);
				m_AssetFileManager->setVisible(false);
			}
		);
	}

	CreateProjectUI::~CreateProjectUI() { }

	void CreateProjectUI::show() {
		if (!m_Visible) return;
		ImGui::Begin("Create Project");

		m_ProjectFileManager->widget();

		m_AssetFileManager->widget();

		//TODO: Script Path

		m_CancelTB->show();
		C78E::GUI::SameLine();
		m_CreateProjectTB->show();

		ImGui::End();

		m_ProjectFileManager->onImGuiRender();
		m_AssetFileManager->onImGuiRender();

	}

	void CreateProjectUI::widget() {
		if (ImGui::Button("Create Project") && !m_Visible) {
			m_Visible = true;
		}
	}

	OpenProjectUI::OpenProjectUI(::C78Editor::GUI::ProjectManager* projectManager)
		: m_ProjectManager(projectManager) {
		m_FileManager = C78E::GUI::FileManager::createOpenDialog(
			m_ProjectManager->getDefaultProjectPath(),
			C78E::FileSystem::EntryType::PROJECT,
			"Open Project :"
		);
		m_Visible = false;
	}

	OpenProjectUI::~OpenProjectUI() { }

	void OpenProjectUI::show() {
		if (!m_Visible) return;

		if (m_FileManager->dialogReady()) {
			if (m_FileManager->getDialogResult().empty()) {
				m_Visible = false;
				return;
			}
			m_ProjectManager->getManager()->openProject(m_FileManager->getDialogResult());
			m_Visible = false;
			return;
		}

		m_FileManager->onImGuiRender();
	}

	void OpenProjectUI::widget() {
		if (ImGui::Button("Open Project") && !m_Visible) {
			m_Visible = true;
		}
	}

	ProjectManagerUI::ProjectManagerUI(::C78Editor::GUI::ProjectManager* projectManager)
		: m_ProjectManager(projectManager), m_CreateProjectUI(projectManager), m_OpenProjectUI(projectManager) {
		m_CreateProjectUI.setVisible(false);
		m_OpenProjectUI.setVisible(false);
	}

	ProjectManagerUI::~ProjectManagerUI() { }

	void ProjectManagerUI::onImGuiRender() {
		if (m_UIState == VIEWPROJECT) return; // no need to show any ProjectManagerUI when there alr is one open

		if (m_UIState == WELCOME)
			onWelcomeScreen();

		if(m_UIState == CREATEPROJECT)
			m_CreateProjectUI.show();

		if(m_UIState == OPENPROJECT)
			m_OpenProjectUI.show();

	}

	void ProjectManagerUI::onWelcomeScreen() {
		if (m_UIState != WELCOME) return; // only show WelcScreen if there is no Project open

		//TODO: design WelcomeScreen
		ImGui::Begin("ProjectManager");

		m_CreateProjectUI.widget();
		m_OpenProjectUI.widget();

		ImGui::Text("Implement latest Projects here!");

		ImGui::End();

	}

}
