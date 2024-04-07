#include "ProjectManager.h"

namespace C78Editor {

	C78E::FilePath ProjectManager::s_CurrentProjectFile = "";
	ProjectManager::UIElement ProjectManager::s_UIElement = ProjectManager::START;


	void ProjectManager::init() {
		s_CurrentProjectFile = "";
	}

	void ProjectManager::onUpdate() {

	}

	void ProjectManager::onImGuiMainMenuBar() {

	}

	bool ProjectManager::onImGuiRender() {
		if (isProjectOpen()) return false;

		glm::vec2 fullWindowSize = { C78E::Application::get().getWindow().getWidth(), C78E::Application::get().getWindow().getHeight() };
		glm::vec2 windowSize = fullWindowSize / 1.6f;
		glm::vec2 windowPos = (fullWindowSize - windowSize) / 2.f;

		ImGui::SetNextWindowPos({ windowPos.x, windowPos.y });
		ImGui::SetNextWindowSize({ windowSize.x, windowSize.y }); // Maximized
		if (!ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) return false;

		switch (s_UIElement) {
		case C78Editor::ProjectManager::START: {
			if (ImGui::Button("Create")) {
				s_UIElement = ProjectManager::CREATE;
			}
			if (ImGui::Button("Open")) {
				s_UIElement = ProjectManager::OPEN;
			}
			} break;
		case C78Editor::ProjectManager::CREATE:
			if (ImGui::Button("Close")) s_UIElement = ProjectManager::START;
			showProjectCreate();
			break;
		case C78Editor::ProjectManager::OPEN:
			if (ImGui::Button("Close")) s_UIElement = ProjectManager::START;
			showProjectOpen();
			break;
		default: break;
		}

		ImGui::End();
		return true;
	}

	void ProjectManager::onProjectCreate(C78E::ProjectConfig config) {
		C78E::Project::create(config);
		onProjectSave();
	}

	void ProjectManager::onProjectOpen() {
		if (isProjectOpen()) onProjectClose();

		s_CurrentProjectFile = C78E::FileDialogs::openFile(C78_FILE_EXT_PROJECT);
		C78E::Project::load(s_CurrentProjectFile);
	}

	void ProjectManager::onProjectSave() {
		if (s_CurrentProjectFile.empty() || !std::filesystem::exists(s_CurrentProjectFile.parent_path())) {
			s_CurrentProjectFile = C78E::FileDialogs::saveFile(C78_FILE_EXT_PROJECT);
			if (s_CurrentProjectFile.empty() || !std::filesystem::exists(s_CurrentProjectFile.parent_path())) return;
		}

		if (s_CurrentProjectFile.extension().empty())
			s_CurrentProjectFile += C78_FILE_EXT_PROJECT;

		C78E::Project::saveActive(s_CurrentProjectFile);
	}

	void ProjectManager::onProjectClose() {
		C78E::Project::close();
		s_CurrentProjectFile = "";
	}
	

	C78E::Ref<ProjectManager::ProjectCreateBuffers> ProjectManager::s_ProjectCreateBuffers = nullptr;
	void ProjectManager::showProjectCreate() {
		if (!s_ProjectCreateBuffers) {
			s_ProjectCreateBuffers = C78E::createRef<ProjectManager::ProjectCreateBuffers>();
			for (size_t off = 0; off < std::max(c_MaxProjectNameSize, c_MaxDirectoryPathSize); off++) {
				if(off < c_MaxProjectNameSize)
					*(s_ProjectCreateBuffers->projectName.data + off) = 0;
				if (off < c_MaxDirectoryPathSize) {
					*(s_ProjectCreateBuffers->assetDirectory.data + off) = 0;
					*(s_ProjectCreateBuffers->assetRegistry.data + off) = 0;
					*(s_ProjectCreateBuffers->scriptModule.data + off) = 0;
				}
			}
		}

		C78E::Buffer& projectName = s_ProjectCreateBuffers->projectName;
		C78E::Buffer& assetDirectory = s_ProjectCreateBuffers->assetDirectory;
		C78E::Buffer& assetRegistry = s_ProjectCreateBuffers->assetRegistry;
		C78E::Buffer& scriptModule = s_ProjectCreateBuffers->scriptModule;
		C78E::ProjectConfig& config = s_ProjectCreateBuffers->config;

		ImGui::InputText("ProjectName", (char*)projectName.data, projectName.size);

		ImGui::InputText("AssetDirectory", (char*)assetDirectory.data, assetDirectory.size);
		ImGui::SameLine();
		ImGui::PushID(1);
		if (ImGui::Button("...")) {
			config.assetDirectory = C78E::FileDialogs::openFolder(getDefaultProjectPath(), C78E::FileDialogs::Flag::HideReadOnly); //TODO: OpenFolder Dialog
			if (assetDirectory.size >= config.assetDirectory.string().size())
				memcpy_s(assetDirectory.data, config.assetDirectory.string().size(), config.assetDirectory.string().c_str(), config.assetDirectory.string().size());
			else C78_EDITOR_ERROR("AssetDirectoryPath is too long, maximun is {}, was {}", assetDirectory.size, config.assetDirectory.string().size());
		}
		ImGui::PopID();

		ImGui::InputText("AssetRegistryName", (char*)assetRegistry.data, assetRegistry.size);
		ImGui::SameLine();
		ImGui::PushID(2);
		if (ImGui::Button("...")) {
			config.assetRegistryPath = C78E::FileDialogs::openFile(C78_FILE_EXT_ASSETREGISTRY, getDefaultProjectPath());
			if (assetRegistry.size >= config.assetRegistryPath.string().size())
				memcpy_s(assetRegistry.data, config.assetRegistryPath.string().size(), config.assetRegistryPath.string().c_str(), config.assetRegistryPath.string().size());
			else C78_EDITOR_ERROR("AssetRegistryName is too long, maximun is {}, was {}", assetRegistry.size, config.assetRegistryPath.string().size());
		}
		ImGui::PopID();

		ImGui::InputText("ScriptModulePath", (char*)scriptModule.data, scriptModule.size);
		
		if (ImGui::Button("Create")) {


			onProjectCreate(config);
		}
	}

	void ProjectManager::showProjectOpen() {
	}

}
