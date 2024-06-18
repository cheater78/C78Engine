#pragma once
#include "Scene.h"

#include <C78E/Assets/AssetManager.h>
#include <C78E/Project/ProjectManager.h>

namespace C78E {

	class SceneManager {
	public:
		SceneManager(Ref<ProjectManager> projectManager);
		~SceneManager();

		Ref<Scene> createScene(const std::string& name); //Editor only
		bool saveScene(SceneHandle sceneHandle = 0, const FilePath& sceneFile = ""); //Editor only
		bool deleteScene(SceneHandle sceneHandle, bool fromDisk = false); //Editor only

		const bool hasActiveScene() const;
		Ref<Scene> getActiveScene();
		void setActiveSceneHandle(SceneHandle sceneHandle);
		SceneHandle getActiveSceneHandle() const;



	private:
		Ref<Scene> validateSceneHandle(SceneHandle sceneHandle);
		Ref<Project> getActiveProject();

		//Editor only
		FilePath fileFromScene(Ref<Scene> scene, Asset::AssetMeta meta);

	private:
		Ref<ProjectManager> m_ProjectManager;

		SceneHandle m_ActiveScene = 0;
	};

}
