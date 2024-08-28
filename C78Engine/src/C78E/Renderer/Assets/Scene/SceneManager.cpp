#include <C78ePCH.h>
#include "SceneManager.h"

#include "SceneSerializer.h"

namespace C78E {

	SceneManager::SceneManager(Ref<ProjectManager> projectManager)
		: m_ProjectManager(projectManager) {
	}

	SceneManager::~SceneManager() { }

	Ref<Scene> SceneManager::createScene(const std::string& name) {
		if (auto projectManager = m_ProjectManager.lock()) {
			Ref<Scene> scene = C78E::createRef<C78E::Scene>(); //Scenes itself are nameless!
			C78E::Asset::AssetMeta meta;
			meta.name = name;
			meta.fileSource = fileFromScene(scene, meta);
			meta.type = C78E::Asset::AssetType::Scene;
			m_ActiveScene = projectManager->getActiveProject()->getEditorAssetManager()->addAsset(meta, scene);
			return projectManager->getActiveProject()->getEditorAssetManager()->getAssetAs<Scene>(m_ActiveScene);
		} else {
			C78_CORE_ERROR("SceneManager::createScene: Called without m_ProjectManager!");
			return nullptr;
		}
	}

	bool SceneManager::saveScene(SceneHandle sceneHandle, const FilePath& sceneFile) {
		if (auto projectManager = m_ProjectManager.lock()) {
			Ref<EditorAssetManager> assetManager = projectManager->getActiveProject()->getEditorAssetManager();
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
		else {
			C78_CORE_ERROR("SceneManager::createScene: Called without m_ProjectManager!");
			return false;
		}
	}

	bool SceneManager::deleteScene(SceneHandle sceneHandle, bool fromDisk) {
		if (auto projectManager = m_ProjectManager.lock()) {
			Ref<Scene> scene = getScene(sceneHandle);
			return projectManager->getActiveProject()->getEditorAssetManager()->removeAsset(sceneHandle, fromDisk);
		}
		else {
			C78_CORE_ERROR("SceneManager::createScene: Called without m_ProjectManager!");
			return false;
		}
	}

	Ref<Scene> SceneManager::getScene(SceneHandle sceneHandle) const {
		if (!sceneHandle) {
			C78_CORE_ERROR("SceneManager::getScene: SceneHandle is null!");
			return nullptr;
		}
		auto projectManager = getProjectManager();
		if (!projectManager) {
			C78_CORE_ERROR("SceneManager::createScene: Called without m_ProjectManager!");
			return nullptr;
		}
		return projectManager->getActiveProject()->getAssetManager()->getAssetAs<Scene>(sceneHandle);
	}

	bool SceneManager::hasActiveScene() const { return m_ActiveScene; }

	Ref<Scene> SceneManager::getActiveScene() const {
		if (auto projectManager = m_ProjectManager.lock()) {
			if (hasActiveScene())
				return getScene(m_ActiveScene);
			else {
				C78_CORE_ERROR("SceneManager::getActiveScene: Called without any Scene active!");
				return nullptr;
			}
		}
		else {
			C78_CORE_ERROR("SceneManager::getActiveScene: Called without m_ProjectManager!");
			return nullptr;
		}
	}

	//should expect and handle null SceneHandles
	void SceneManager::setActiveSceneHandle(SceneHandle sceneHandle) {
		if (auto projectManager = getProjectManager()) {
			if (sceneHandle && !getScene(sceneHandle)) {
				C78_CORE_ERROR("SceneManager::setActiveSceneHandle: SceneHandle provided was not null, but does not associate an existing Scene!");
				return;
			}
			m_ActiveScene = sceneHandle;
		}
		else {
			C78_CORE_ERROR("SceneManager::setActiveSceneHandle: Called without m_ProjectManager!");
			return;
		}
	}

	SceneHandle SceneManager::getActiveSceneHandle() const { return m_ActiveScene; }
	bool SceneManager::activeSceneIsEmpty() const { return hasActiveScene() && getActiveScene()->isEmpty(); }



	/*
	* Expose ProjectManager
	*/
	bool SceneManager::hasActiveProjectManager() const { return !m_ProjectManager.expired(); }

	Ref<ProjectManager> SceneManager::getProjectManager() const {
		if (hasActiveProjectManager())
			return m_ProjectManager.lock();
		else {
			C78_CORE_ERROR("SceneManager::getProjectManager: Called without m_ProjectManager!");
			return nullptr;
		}
	}

	bool SceneManager::hasActiveProject() const {
		if (auto projectManager = getProjectManager()) {
			return projectManager->hasActiveProject();
		}
		else {
			C78_CORE_ERROR("SceneManager::hasActiveProject: Called without m_ProjectManager!");
			return false;
		}
	}

	Ref<Project> SceneManager::getActiveProject() const {
		if (auto projectManager = getProjectManager()) {
			if (projectManager->hasActiveProject()) {
				return projectManager->getActiveProject();
			}
			else {
				C78_CORE_ERROR("SceneManager::getActiveProject: ProjectManager has no active project!");
				return nullptr;
			}
		}
		else {
			C78_CORE_ERROR("SceneManager::getActiveProject: Called without m_ProjectManager!");
			return nullptr;
		}
	}

	bool SceneManager::hasActiveProjectFile() const {
		if (auto projectManager = getProjectManager()) {
			return projectManager->hasActiveProjectFile();
		}
		else {
			C78_CORE_ERROR("SceneManager::hasActiveProject: Called without m_ProjectManager!");
			return false;
		}
	}


	

	
	FilePath SceneManager::fileFromScene(Ref<Scene> scene, Asset::AssetMeta meta) {
		if (auto projectManager = getProjectManager()) {
			return projectManager->getActiveProject()->getAssetDirectory() / C78E_ASSETLOC_SCENE / (meta.name + "-" + std::to_string(scene->m_AssetHandle) + C78E_FILE_EXT_SCENE);
		}
		else {
			C78_CORE_ERROR("SceneManager::fileFromScene: Called without m_ProjectManager!");
			return "";
		}
	}

}