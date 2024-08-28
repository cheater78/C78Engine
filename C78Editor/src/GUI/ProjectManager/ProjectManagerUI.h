#pragma once

#include <C78E.h>
#include <C78Egui.h>
#include <C78Elibs.h>

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

	private:
		C78E::WRef<C78E::ProjectManager> m_ProjectManager;

		struct ProjectCreateConfigBuffers {
		public:
			C78E::Buffer projectName{64};
			C78E::Buffer projectDirectory{256};
			C78E::Buffer assetDirectory{256};
			C78E::Buffer assetRegistry{256};
			C78E::Buffer scriptModulePath{256};
		public:
			C78E::FilePath getProjectFilePath() {
				C78E::FilePath filepath = C78E::FilePath(std::string(projectDirectory.as<char>())) /
					C78E::FilePath(std::string(projectName.as<char>()) + C78E_FILE_EXT_PROJECT);
				if (!C78E::FileSystem::isFile(filepath)) {
					C78_EDITOR_ERROR("ProjectManagerUI::ProjectCreateConfigBuffers::getProjectFilePath: Input is not a FilePath! {}", filepath.string());
					return "";
				}
				return filepath;
			}




		} m_PCCB;
	};

}
