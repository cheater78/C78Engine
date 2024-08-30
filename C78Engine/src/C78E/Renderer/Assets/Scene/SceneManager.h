#pragma once
#include "Scene.h"

#include <C78E/Renderer/Assets/AssetManager.h>
#include <C78E/Project/ProjectManager.h>

namespace C78E {

	class SceneManager {
	public:
		SceneManager(Ref<ProjectManager> projectManager);
		~SceneManager();

		Ref<Scene> createScene(const std::string& name = "Unnamed Scene");
		bool saveScene(SceneHandle sceneHandle = 0, const FilePath& sceneFile = "");
		bool deleteScene(SceneHandle sceneHandle, bool fromDisk = false);

		Ref<Scene> getScene(SceneHandle sceneHandle) const;


		bool hasActiveScene() const;
		Ref<Scene> getActiveScene() const;
		void setActiveSceneHandle(SceneHandle sceneHandle);
		SceneHandle getActiveSceneHandle() const;
		bool activeSceneIsEmpty() const;

	public:	//Expose ProjectManager
		Ref<ProjectManager> getProjectManager() const;
		bool hasActiveProject() const;
		Ref<Project> getActiveProject() const;
		bool hasActiveProjectFile() const;
		FilePath getActiveProjectFile() const;

	private:
		// Naming for Serialized Scene Files
		FilePath fileFromScene(Ref<Scene> scene, Asset::AssetMeta meta);

	private:
		Ref<ProjectManager> m_ProjectManager;
		SceneHandle m_ActiveScene = (uint64_t)0;

	};

}
