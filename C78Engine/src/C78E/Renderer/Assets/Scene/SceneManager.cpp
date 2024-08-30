#include <C78ePCH.h>
#include "SceneManager.h"

#include "SceneSerializer.h"

namespace C78E {

	SceneManager::SceneManager(Ref<ProjectManager> projectManager)
		: m_ProjectManager(projectManager) {
	}

	SceneManager::~SceneManager() { }

	Ref<Scene> SceneManager::createScene(const std::string& name) {
		Ref<Scene> scene = C78E::createRef<C78E::Scene>(); //Scenes itself are nameless!
		C78E::Asset::AssetMeta meta;
		meta.name = name;
		meta.fileSource = fileFromScene(scene, meta);
		meta.type = C78E::Asset::AssetType::Scene;
		m_ActiveScene = m_ProjectManager->getActiveProject()->getEditorAssetManager()->addAsset(meta, scene);
		return m_ProjectManager->getActiveProject()->getEditorAssetManager()->getAssetAs<Scene>(m_ActiveScene);
	}

	bool SceneManager::saveScene(SceneHandle sceneHandle, const FilePath& sceneFile) {
		Ref<EditorAssetManager> assetManager = m_ProjectManager->getActiveProject()->getEditorAssetManager();
		if (!sceneHandle)
			sceneHandle = m_ActiveScene;
		Asset::AssetMeta meta = assetManager->getMeta(sceneHandle);
		if (sceneFile.empty()) {
			if (meta.fileSource.empty()) {
				C78_CORE_ERROR("SceneManager::saveScene: AssetMeta FileSource empty, so is arg sceneFile, atleast one must be set!");
				return false;
			}
		} else {
			meta.fileSource = sceneFile;
		}
		C78E::SceneSerializer serializer(std::static_pointer_cast<C78E::Scene>(assetManager->getAsset(sceneHandle)), meta);
		serializer.serialize();
		return true;
	}

	bool SceneManager::deleteScene(SceneHandle sceneHandle, bool fromDisk) {
		Ref<Scene> scene = getScene(sceneHandle);
		return m_ProjectManager->getActiveProject()->getEditorAssetManager()->removeAsset(sceneHandle, fromDisk);
	}

	Ref<Scene> SceneManager::getScene(SceneHandle sceneHandle) const {
		if (!sceneHandle) {
			C78_CORE_ERROR("SceneManager::getScene: SceneHandle is null!");
			return nullptr;
		}
		return m_ProjectManager->getActiveProject()->getAssetManager()->getAssetAs<Scene>(sceneHandle);
	}

	bool SceneManager::hasActiveScene() const {
		if (m_ProjectManager->hasActiveProject())
			return m_ActiveScene;
		return false;
	}

	Ref<Scene> SceneManager::getActiveScene() const {
		if (hasActiveScene())
			return getScene(m_ActiveScene);
		else {
			C78_CORE_ERROR("SceneManager::getActiveScene: Called without any Scene active!");
			return nullptr;
		}
	}

	//should expect and handle null SceneHandles
	void SceneManager::setActiveSceneHandle(SceneHandle sceneHandle) {
		if (sceneHandle && !getScene(sceneHandle)) {
			C78_CORE_ERROR("SceneManager::setActiveSceneHandle: SceneHandle provided was not null, but does not associate an existing Scene!");
			return;
		}
		m_ActiveScene = sceneHandle;
	}

	SceneHandle SceneManager::getActiveSceneHandle() const { return m_ActiveScene; }
	bool SceneManager::activeSceneIsEmpty() const { return hasActiveScene() && getActiveScene()->isEmpty(); }



	/*
	* Expose ProjectManager
	*/

	Ref<ProjectManager> SceneManager::getProjectManager() const { return m_ProjectManager; }

	bool SceneManager::hasActiveProject() const { return m_ProjectManager->hasActiveProject(); }

	Ref<Project> SceneManager::getActiveProject() const {
		if (m_ProjectManager->hasActiveProject()) {
			return m_ProjectManager->getActiveProject();
		}
		else {
			C78_CORE_ERROR("SceneManager::getActiveProject: ProjectManager has no active project!");
			return nullptr;
		}
	}

	bool SceneManager::hasActiveProjectFile() const { return m_ProjectManager->hasActiveProjectFile(); }


	

	
	FilePath SceneManager::fileFromScene(Ref<Scene> scene, Asset::AssetMeta meta) {
		return m_ProjectManager->getActiveProject()->getAssetDirectory() / C78E_ASSETLOC_SCENE / (meta.name + "-" + std::to_string(scene->m_AssetHandle) + C78E_FILE_EXT_SCENE);
	}

}