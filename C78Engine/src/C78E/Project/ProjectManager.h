#pragma once

#include <C78E/Project/Project.h>

namespace C78E {
	/*
	* C78E::ProjectManager -> the Logic to create or open projects
	*/
	class ProjectManager {
	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		Ref<Project> createProject(const FilePath& projectDirectoryPath, Project::Config config, Ref<AssetManager> assetManager = nullptr);
		Ref<Project> openProject(const FilePath& projectFile);
		bool saveProject(const FilePath& projectFile = "");
		bool closeProject(bool save = true);
		bool removeProject(const FilePath& projectFile);

		bool hasActiveProject() const;
		Ref<Project> getActiveProject() const;
		bool hasActiveProjectFile() const;
		FilePath getActiveProjectFile() const;

		Ref<EditorAssetManager> getEditorAssetManager();

		Ref<Project> reloadProject();

	private:
		Ref<Project> m_ActiveProject = nullptr;
		FilePath m_ActiveProjectFile; //somewhat redundant to Project::ProjectDirectory
	};

}



