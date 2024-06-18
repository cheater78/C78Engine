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

		const bool hasActiveProject() const;
		const Ref<Project> getActiveProject() const;
		const bool hasActiveProjectFile() const;

		Ref<Project> reloadProject();

	private:
		Ref<Project> m_ActiveProject = nullptr;
		FilePath m_ActiveProjectFile;
	};

}



