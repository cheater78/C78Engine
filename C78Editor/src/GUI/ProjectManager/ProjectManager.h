#pragma once

#include <C78E.h>
#include <C78Elibs.h>
#include <C78Egui.h>

#include "ProjectManagerUI.h"

namespace C78Editor::GUI {
	/*
	* C78Editor::GUI::ProjectManager -> the Logic and UI that allows the user to create or open projects
	*/
	class ProjectManager {
	public:
		ProjectManager();
		//ProjectManager(C78E::Ref<C78E::ProjectManager> projectManager); // not needed?
		~ProjectManager();

		void onUpdate(C78E::Timestep dt);

		void onImGuiMainMenuBar();
		void onImGuiRender();

	public: // shouldnt be
		C78E::Ref<C78E::ProjectManager> getManager() const;
		static C78E::FilePath getDefaultProjectPath() { return "../C78Project"; }
	private:
		C78E::Ref<C78E::ProjectManager> m_ProjectManager;
		C78E::Ref<ProjectManagerUI> m_UI;
	};

}


