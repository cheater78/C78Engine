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
			C78E::GUI::TextInput projectName{"Project Name ", "C78Project"};
			C78E::GUI::TextInput projectDirectory{ "Project Directory ", "./" };
			C78E::GUI::TextButton saveProjectFile{ "...", [this](void) -> void { 
				C78E::FilePath path = C78E::FileDialogs::saveFile(
					"C78Project (.pce)\0*.pce\0",
					getProjectFilePath().parent_path(),
					getProjectFilePath());
				if (!path.empty())
					projectDirectory.setContent(path.parent_path().string());
			} };
			C78E::GUI::TextInput assetDirectory{ "Asset Directory ", "./assets" };
			C78E::GUI::TextInput assetRegistry{ "Asset Registry ", "AssetRegistry" };
			C78E::GUI::TextButton saveAssetRegistryFile{ "...", [this](void) -> void {
				C78E::FilePath path = C78E::FileDialogs::saveFile(
					"C78AssetRegistry (.ace)\0*.ace\0",
					getAssetDirectoryPath(),
					(getAssetRegistryFilePath().is_relative()) 
					? std::filesystem::absolute(getAssetDirectoryPath() / getAssetRegistryFilePath()) 
					: getAssetRegistryFilePath());
				if (!path.empty())
					assetDirectory.setContent(path.parent_path().string());
			} };
			C78E::GUI::TextInput scriptModulePath{ "Script Module Path ", "./scripts" };
		public:

			std::string getProjectName() const { return projectName.getContent(); }
			C78E::FilePath getProjectFilePath() const {
				C78E::FilePath filepath = C78E::FilePath(projectDirectory.getContent()) /
					C78E::FilePath(projectName.getContent() + C78E_FILE_EXT_PROJECT);
				if (filepath.is_relative()) filepath = std::filesystem::absolute(filepath);
				if (filepath.empty()) {
					C78_EDITOR_ERROR("ProjectManagerUI::ProjectCreateConfigBuffers::getProjectFilePath: Input is not a FilePath! {}", filepath.string());
					return "";
				}
				return filepath;
			}
			C78E::FilePath getAssetDirectoryPath() const {
				C78E::FilePath filepath = C78E::FilePath(assetDirectory.getContent());
				if (filepath.is_relative()) {
					filepath = (getProjectFilePath().parent_path() / filepath);
					filepath = std::filesystem::absolute(filepath);
				}
				if (filepath.empty()) {
					C78_EDITOR_ERROR("ProjectManagerUI::ProjectCreateConfigBuffers::getAssetDirectoryPath: Input is not a FilePath! {}", filepath.string());
					return "";
				}
				return filepath;
			}
			C78E::FilePath getAssetRegistryFilePath() {
				C78E::FilePath filepath = C78E::FilePath(assetRegistry.getContent());
				if (filepath.extension().empty())
					filepath += C78E_FILE_EXT_ASSETREGISTRY;
				if (filepath.empty()) {
					C78_EDITOR_ERROR("ProjectManagerUI::ProjectCreateConfigBuffers::getAssetDirectoryPath: Input is not a FilePath! {}", filepath.string());
					return "";
				}
				if (filepath.is_relative())
					filepath = getAssetDirectoryPath() / filepath;
				return filepath;
			}
			C78E::FilePath getScriptModulePath() {
				return C78E::FilePath(scriptModulePath.getContent());
			}
		} m_PCCB;




	};

}
