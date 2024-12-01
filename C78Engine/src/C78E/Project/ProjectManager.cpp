#include "C78EPCH.h"
#include "ProjectManager.h"

namespace C78E {

	/*
	* Creates a Project, switches to it, saves the old one if existing
	*/
	Ref<Project> ProjectManager::createProject(const FilePath& projectDirectoryPath, Project::Config config, Ref<AssetManager> assetManager) {
		//input validation / check Mmg redundancy
		if (m_ActiveProject) closeProject(true);
		m_ActiveProject = Project::create(projectDirectoryPath, config, assetManager);
		return m_ActiveProject;
	}

	/*
	* Opens a Project, switches to it, saves the old one if existing
	*/
	Ref<Project> ProjectManager::openProject(const FilePath& projectFile) {
		if (m_ActiveProject) closeProject(true);
		m_ActiveProject = Project::load(projectFile);
		if (m_ActiveProject)
			m_ActiveProjectFile = projectFile;
		return m_ActiveProject;
	}

	bool ProjectManager::saveProject(const FilePath& projectFile) {
		C78E_CORE_ASSERT(m_ActiveProject, "ProjectManager::saveProject: ActiveProject is not set!");
		if (!m_ActiveProject) return false;

		if (projectFile.empty()) {
			C78E_CORE_ASSERT(!m_ActiveProjectFile.empty(), "ProjectManager::saveProject: m_ActiveProjectFile is not set!");
			if (m_ActiveProjectFile.empty()) {
				C78E::FilePath path = FileDialogs::saveFile({C78E::FileSystem::EntryType::Project}, C78E::FileSystem::C78RootDirectory, "", "C78E Save Project");
				if(path.empty())
					return false;
				m_ActiveProjectFile = path;
			}
		} else {
			m_ActiveProjectFile = projectFile;
		}
		return Project::save(m_ActiveProject, m_ActiveProjectFile);
	}

	bool ProjectManager::closeProject(bool save) {
		C78E_CORE_ASSERT(m_ActiveProject, "ProjectManager::closeProject: ActiveProject is not set!");
		if (!m_ActiveProject) return false;
		
		if (save)
			if (!saveProject())
				return false;
		m_ActiveProject = nullptr;
		m_ActiveProjectFile.clear();
		return true;
	}

	bool ProjectManager::hasActiveProject() const {
		return m_ActiveProject.get() != nullptr;
	}

	Ref<Project> ProjectManager::getActiveProject() const {
		if (hasActiveProject()) {
			return m_ActiveProject;
		}
		else {
			C78E_CORE_ERROR("ProjectManager::getActiveProject: Called but no active Project!");
			return nullptr;
		}
	}

	bool ProjectManager::hasActiveProjectFile() const {
		return !m_ActiveProjectFile.empty();
	}

	FilePath ProjectManager::getActiveProjectFile() const {
		return m_ActiveProjectFile;
	}

	Ref<EditorAssetManager> ProjectManager::getEditorAssetManager() {
		if (hasActiveProject())
			return getActiveProject()->getEditorAssetManager();
		return nullptr;
	}

	Ref<Project> ProjectManager::reloadProject() {
		return Project::load(m_ActiveProjectFile);
	}

}


