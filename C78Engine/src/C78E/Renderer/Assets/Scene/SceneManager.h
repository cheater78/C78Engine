#pragma once
#include "Scene.h"

#include <C78E/Renderer/Assets/AssetManager.h>
#include <C78E/Project/ProjectManager.h>

namespace C78E {

	class SceneManager {
	public:
		SceneManager(Ref<ProjectManager> projectManager);
		SceneManager(const SceneManager& other) = delete;
		~SceneManager() = default;

		Ref<Scene> createScene(const std::string& name = C78E_DEFAULT_SCENE_NAME, bool allowFileOverride = false);
		bool saveScene(SceneHandle sceneHandle = AssetHandle::invalid(), const FilePath& sceneFile = "", bool overrideMetaSource = true);
		bool deleteScene(SceneHandle sceneHandle, bool fromDisk = false);

		Ref<Scene> getScene(SceneHandle sceneHandle) const;
		
		bool hasActiveScene() const;
		Ref<Scene> getActiveScene() const;
		bool setActiveSceneHandle(SceneHandle sceneHandle);
		SceneHandle getActiveSceneHandle() const;
		bool activeSceneIsEmpty() const;

	public:	//Expose ProjectManager for easy access
		Ref<ProjectManager> getProjectManager() const;
		bool hasActiveProject() const;
		Ref<Project> getActiveProject() const;
		bool hasActiveProjectFile() const;
		FilePath getActiveProjectFile() const;

	private:
		FilePath getNewAvailableSceneFile(Ref<Scene> scene, const std::string& name, bool allowOverride = false);
	private:
		Ref<ProjectManager> m_ProjectManager;
		SceneHandle m_ActiveScene = AssetHandle::invalid();
	};

}
