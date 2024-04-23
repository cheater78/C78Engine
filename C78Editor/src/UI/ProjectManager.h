#pragma once

#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class ProjectManager {
	public:

		static void init();
		static void onUpdate();

		static void onImGuiMainMenuBar();
		static bool onImGuiRender();
		
		static void onProjectOpen();
		static void onProjectSave();
		static void onProjectClose();

	private:
		static bool isProjectOpen() { return (bool)C78E::Project::getActive(); }
		static C78E::FilePath getDefaultProjectPath() { return "../C78Project"; }

	private:
		static C78E::FilePath s_CurrentProjectFile;
		static C78E::Ref<C78E::FileManager> s_FileManager;
		static std::string s_FileManagerOwner;
		static C78E::FilePath s_FileManagerResult; // unused
	private:
		static void showProjectCreate();



		static void showProjectOpen();


	private:
		enum UIElement {
			START,
			CREATE,
			OPEN
		};

		static UIElement s_UIElement;
	};

}


