#include "ProjectManager.h"

namespace C78Editor::GUI {

	ProjectManager::ProjectManager()
		: m_ProjectManager(C78E::createRef<::C78E::ProjectManager>()) {
		m_UI = C78E::createRef<ProjectManagerUI>(this);
	}

	ProjectManager::~ProjectManager() { }

	void ProjectManager::onUpdate(C78E::Timestep dt) {

	}

	void ProjectManager::onImGuiMainMenuBar() {
		if (ImGui::BeginMenu("Project")) {

			if (ImGui::MenuItem("New")) {

			}
			if (ImGui::MenuItem("Open", "")) {}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("impl!", "", nullptr, false);

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "", nullptr, getManager()->hasActiveProjectFile())) {
				getManager()->saveProject(); // Active Project File is alr set
			}

			ImGui::EndMenu();
		}
	}

	void ProjectManager::onImGuiRender() {
		m_UI->onImGuiRender();
	}

	C78E::Ref<C78E::ProjectManager> ProjectManager::getManager() const { return m_ProjectManager; }

}
