#include "C78ePCH.h"
#include "Project.h"
#include "ProjectSerializer.h"

namespace C78E {

	FilePath Project::getAssetAbsolutePath(const FilePath& path) {
		return getAssetDirectory() / path;
	}

	Ref<Project> Project::create() {
		s_ActiveProject = createRef<Project>();
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
			return true;
		}
		return false;
	}

}