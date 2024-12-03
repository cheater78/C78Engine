#include <C78EPCH.h>
#include "SceneManager.h"

#include <C78E/Renderer/Assets/Scene/SceneSerializer.h>

namespace C78E {

	/*
	* Constructor for SceneManager, assures a valid, needed, ProjectManager
	*/
	SceneManager::SceneManager(Ref<ProjectManager> projectManager)
		: m_ProjectManager(projectManager) {
		C78E_CORE_ASSERT(m_ProjectManager, "SceneManager::SceneManager: ProjectManager cannot be null!");
	}

	/*
	* Creates a new Scene, assigns the chosen name to the Meta, generates a Path for Serialization, sets it as ActiveScene and returns it
	*/
	Ref<Scene> SceneManager::createScene(const std::string& name, bool allowFileOverride) {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return nullptr, "SceneManager::createScene: No active Project!")
		Ref<Scene> scene = C78E::createRef<C78E::Scene>();
		Ref<Asset::Meta> meta;
		meta->name = name;
		meta->fileSource = FileSystem::getRelativePathTo(getNewAvailableSceneFile(scene, name, allowFileOverride), m_ProjectManager->getActiveProject()->getAssetDirectory());
		meta->type = C78E::Asset::Type::Scene;
		m_ActiveScene = m_ProjectManager->getEditorAssetManager()->addAsset(meta, scene);
		return m_ProjectManager->getEditorAssetManager()->getAssetAs<Scene>(m_ActiveScene);
	}

	/*
	* Saves an existing Scene
	*/
	bool SceneManager::saveScene(SceneHandle sceneHandle, const FilePath& sceneFile, bool overrideMetaSource) {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return false, "SceneManager::saveScene: No active Project!");
		Ref<EditorAssetManager> assetManager = m_ProjectManager->getEditorAssetManager();
		if (!sceneHandle)
			sceneHandle = m_ActiveScene;
		C78E_CORE_VALIDATE(assetManager->isValid(sceneHandle), return false, "SceneManager::saveScene: SceneHandle is not associated with a Scene! SceneHandle: {}", sceneHandle);
		
		Ref<Asset::Meta> meta = assetManager->getMeta(sceneHandle);
		C78E_CORE_VALIDATE(!(sceneFile.empty() && meta->fileSource.empty()), return false, "SceneManager::saveScene: Meta FileSource and sceneFile empty, atleast one must be set!")
		if(!sceneFile.empty() && overrideMetaSource)
			meta->fileSource = sceneFile;

		return SceneSerializer::exportScene(assetManager->getAssetAs<Scene>(sceneHandle), m_ProjectManager->getActiveProject()->getAssetDirectory() / FilePath((sceneFile.empty()) ? meta->fileSource : sceneFile));
	}

	/*
	* Deletes an existing Scene
	*/
	bool SceneManager::deleteScene(SceneHandle sceneHandle, bool fromDisk) {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return false, "SceneManager::deleteScene: No active Project!");
		C78E_CORE_VALIDATE(m_ProjectManager->getEditorAssetManager()->isValid(sceneHandle), return false, "SceneManager::deleteScene: SceneHandle is not associated with a Scene! SceneHandle: {}", sceneHandle);
		return m_ProjectManager->getEditorAssetManager()->removeAsset(sceneHandle, fromDisk);
	}

	/*
	* Provides an existing Scene
	*/
	Ref<Scene> SceneManager::getScene(SceneHandle sceneHandle) const {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return nullptr, "SceneManager::getScene: No active Project!");
		C78E_CORE_VALIDATE(m_ProjectManager->getEditorAssetManager()->isValid(sceneHandle), return nullptr, "SceneManager::getScene: SceneHandle is not associated with a Scene! SceneHandle: {}", sceneHandle);
		return m_ProjectManager->getEditorAssetManager()->getAssetAs<Scene>(sceneHandle);
	}

	/*
	* Determines wheter this SceneManager has an active Scene
	*/
	bool SceneManager::hasActiveScene() const {
		return m_ProjectManager->hasActiveProject() && m_ActiveScene.isValid();
	}

	/*
	* Provides access to this SceneManager's active Scene
	*/
	Ref<Scene> SceneManager::getActiveScene() const {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return nullptr, "SceneManager::getActiveScene: No active Project!");
		C78E_CORE_VALIDATE(hasActiveScene(), return nullptr, "SceneManager::getActiveScene: No active Scene!");
		return getScene(m_ActiveScene);
	}

	/*
	* Sets this SceneManager's active Scene by SceneHandle, NullHandle unsets the active Scene
	*/
	bool SceneManager::setActiveSceneHandle(SceneHandle sceneHandle) {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return false, "SceneManager::getActiveScene: No active Project!");
		C78E_CORE_VALIDATE(!sceneHandle || m_ProjectManager->getEditorAssetManager()->isValid(sceneHandle), return false, "SceneManager::getActiveScene: SceneHandle is not associated with a Scene! SceneHandle: {}", sceneHandle);
		m_ActiveScene = sceneHandle;
		return true;
	}

	/*
	* Provides access to this SceneManager's active SceneHandel
	*/
	SceneHandle SceneManager::getActiveSceneHandle() const {
		return m_ActiveScene;
	}

	/*
	* Determines whether there is any Content in the active Scene, false if no active Scene
	*/
	bool SceneManager::activeSceneIsEmpty() const {
		return hasActiveScene() && getActiveScene()->isEmpty();
	}



	/*
	* Provides access to the underlying ProjectManager
	*/
	Ref<ProjectManager> SceneManager::getProjectManager() const {
		C78E_CORE_VALIDATE(m_ProjectManager, return nullptr, "SceneManager::getProjectManager: ProjectManager is null!");
		return m_ProjectManager;
	}

	/*
	* Determines whether the underlying ProjectManager has an active Project
	*/
	bool SceneManager::hasActiveProject() const {
		C78E_CORE_VALIDATE(m_ProjectManager, return false, "SceneManager::hasActiveProject: ProjectManager is null!");
		return m_ProjectManager->hasActiveProject();
	}

	/*
	* Provides access to the underlying ProjectManager's active Project
	*/
	Ref<Project> SceneManager::getActiveProject() const {
		C78E_CORE_VALIDATE(m_ProjectManager, return nullptr, "SceneManager::getActiveProject: ProjectManager is null!");
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return nullptr, "SceneManager::getActiveProject: ProjectManager does not have an active Project!");
		return m_ProjectManager->getActiveProject();
	}

	/*
	* Determines whether the underlying ProjectManager has an active Project File
	*/
	bool SceneManager::hasActiveProjectFile() const {
		C78E_CORE_VALIDATE(m_ProjectManager, return false, "SceneManager::hasActiveProjectFile: ProjectManager is null!");
		return m_ProjectManager->hasActiveProject() &&  m_ProjectManager->hasActiveProjectFile();
	}


	/*
	* Provides a Scene FilePath for saving Scenes
	*/
	FilePath SceneManager::getNewAvailableSceneFile(Ref<Scene> scene, const std::string& name, bool allowOverride) {
		const FilePath absoluteBase = m_ProjectManager->getActiveProject()->getAssetDirectory() / C78E_ASSETLOCATION_SCENE / name;
		FilePath newAvailableSceneFile = absoluteBase.string() + C78E_FILE_EXT_SCENE;
		for (uint64_t i = 2; !allowOverride && C78E::FileSystem::exists(newAvailableSceneFile); i++)
			newAvailableSceneFile = absoluteBase.string() + std::to_string(i) + C78E_FILE_EXT_SCENE;
		return newAvailableSceneFile;
	}

}