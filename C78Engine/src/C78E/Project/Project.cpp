#include "C78ePCH.h"
#include "Project.h"
#include "ProjectSerializer.h"

namespace C78E {

	FilePath Project::getAssetAbsolutePath(const FilePath& path) {
		return getAssetDirectory() / path;
	}

	Ref<Project> Project::create(ProjectConfig config) {
		s_ActiveProject = createRef<Project>();
		s_ActiveProject->m_Config = config;

		s_ActiveProject->m_AssetManager = createRef<EditorAssetManager>();

		if (std::filesystem::exists(config.assetDirectory) && std::filesystem::exists(config.assetDirectory / config.assetRegistryPath))
			s_ActiveProject->getEditorAssetManager()->deserializeAssetRegistry();

		return s_ActiveProject;
	}

	Ref<Project> Project::load(const FilePath& path) {
		Ref<Project> project = createRef<Project>();

		ProjectSerializer serializer(project);
		if (serializer.deserializeProject(path)) {
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			Ref<EditorAssetManager> editorAssetManager = createRef<EditorAssetManager>();
			s_ActiveProject->m_AssetManager = editorAssetManager;
			editorAssetManager->deserializeAssetRegistry();
			return s_ActiveProject;
		}
		return nullptr;
	}

	bool Project::saveActive(const FilePath& path) {
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.serializeProject(path)) {
			s_ActiveProject->m_ProjectDirectory = path.parent_path();

			if (!std::filesystem::exists(s_ActiveProject->m_Config.assetDirectory) || !std::filesystem::is_directory(s_ActiveProject->m_Config.assetDirectory))
				std::filesystem::create_directories(s_ActiveProject->m_Config.assetDirectory);

			if (!std::filesystem::exists(getActiveAssetRegistryPath().parent_path()) || !std::filesystem::is_directory(getActiveAssetRegistryPath().parent_path()))
				std::filesystem::create_directories(getActiveAssetRegistryPath().parent_path());

			s_ActiveProject->getEditorAssetManager()->serializeAssetRegistry();

			return true;
		}
		return false;
	}

}