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

		static void onProjectCreate(C78E::ProjectConfig config = C78E::ProjectConfig());
		static void onProjectOpen();
		static void onProjectSave();
		static void onProjectClose();

	private:
		static bool isProjectOpen() { return (bool)C78E::Project::getActive(); }
		static C78E::FilePath getDefaultProjectPath() { return "C:\\dev\\c-cpp\\C78Engine\\C78Project"; }

	private:
		static C78E::FilePath s_CurrentProjectFile;

	private:
		static constexpr uint64_t c_MaxProjectNameSize = 64;
		static constexpr uint64_t c_MaxDirectoryPathSize = 1024;
		struct ProjectCreateBuffers {
			C78E::Buffer projectName = C78E::Buffer(c_MaxProjectNameSize);
			C78E::Buffer assetDirectory = C78E::Buffer(c_MaxDirectoryPathSize);
			C78E::Buffer assetRegistry = C78E::Buffer(c_MaxDirectoryPathSize);
			C78E::Buffer scriptModule = C78E::Buffer(c_MaxDirectoryPathSize);
			C78E::ProjectConfig config = {"Untitled Project", 0, "", "", ""};
		};
		static C78E::Ref<ProjectCreateBuffers> s_ProjectCreateBuffers;
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


