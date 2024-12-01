#pragma once

#include <C78E.h>

namespace C78Editor::GUI {

	class ProjectManagerUI {
	public:
		ProjectManagerUI(C78E::Ref<C78E::ProjectManager> projectManager);
		~ProjectManagerUI();

		virtual void onImGuiMainMenuBar();
		virtual void onImGuiRender();

	private:
		void drawNoProject();
		void drawCreateProject();
		void drawProjectInfo();

		void startCreateProcess();
		void abortCreateProcess();

	private:
		C78E::WRef<C78E::ProjectManager> m_ProjectManager;

		bool m_ProjectCreateInProgress = false;
		C78E::FilePath m_ProjectCreateDirectory = C78E::FileSystem::C78RootDirectory;
		C78E::Project::Config m_ProjectCreateConfig{};
	};

}
