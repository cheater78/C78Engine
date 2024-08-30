#pragma once

#include <C78E/Project/Project.h>

#define C78E_ASSETLOC_SCENE "scenes"

namespace C78E {
	/*
	* C78E::ProjectManager -> the Logic to create or open projects
	*/
	class ProjectManager {
	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		Ref<Project> createProject(ProjectConfig config);
		Ref<Project> openProject(const FilePath& projectFile);
		bool saveProject(const FilePath& projectFile = "");
		bool closeProject(bool save = true);
		bool removeProject(const FilePath& projectFile);

		bool hasActiveProject() const;
		Ref<Project> getActiveProject() const;
		bool hasActiveProjectFile() const;
		FilePath getActiveProjectFile() const;

		Ref<Project> reloadProject();

	private:
		Ref<Project> m_ActiveProject = nullptr;
		FilePath m_ActiveProjectFile;
	};

}



