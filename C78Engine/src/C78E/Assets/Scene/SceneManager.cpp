#include <C78ePCH.h>
#include "SceneManager.h"

#include "SceneSerializer.h"

namespace C78E {

	SceneManager::SceneManager(Ref<ProjectManager> projectManager)
		: m_ProjectManager(projectManager) {
	}

	SceneManager::~SceneManager() { }

	Ref<Scene> SceneManager::createScene(const std::string& name) {
		Ref<Scene> scene = C78E::createRef<C78E::Scene>(); //Scenes are nameless!
		C78E::Asset::AssetMeta meta;
		meta.name = name;
		meta.fileSource = fileFromScene(scene, meta);
		meta.type = C78E::Asset::AssetType::Scene;
		m_ActiveScene = m_ProjectManager->getActiveProject()->getEditorAssetManager()->addAsset(meta, scene);
		return std::static_pointer_cast<Scene>(m_ProjectManager->getActiveProject()->getEditorAssetManager()->getAsset(m_ActiveScene));
	}

	bool SceneManager::saveScene(SceneHandle sceneHandle, const FilePath& sceneFile) {
		Ref<EditorAssetManager> assetManager = getActiveProject()->getEditorAssetManager();
		if (!sceneHandle)
			sceneHandle = m_ActiveScene;
		Asset::AssetMeta meta = assetManager->getMeta(sceneHandle);
		if (sceneFile.empty()) {
			C78_CORE_ASSERT(!meta.fileSource.empty(), "SceneManager::saveScene: AssetMeta FileSource empty, but sceneFile path arg as well!");
		} else {
			meta.fileSource = sceneFile;
		}
		C78E::SceneSerializer serializer(std::static_pointer_cast<C78E::Scene>(assetManager->getAsset(sceneHandle)), meta);
		serializer.serialize();
		return true;
	}

	bool SceneManager::deleteScene(SceneHandle sceneHandle, bool fromDisk) {
		Ref<Scene> scene = validateSceneHandle(sceneHandle);
		if (scene) {
			C78_CORE_ERROR("SceneManager::deleteScene: not implemented in EditorAssetManager!");
			return false;
			//return m_ProjectManager->getActiveProject()->getEditorAssetManager()->removeAsset(fromDisk);
		} else {
			C78_CORE_ERROR("SceneManager::deleteScene: SceneHandle is null!");
			C78_CORE_ASSERT(false);
			return false;
		}
	}

	const bool SceneManager::hasActiveScene() const {
		return m_ActiveScene;
	}

	Ref<Scene> SceneManager::getActiveScene() {
		return validateSceneHandle(m_ActiveScene);
	}

	void SceneManager::setActiveSceneHandle(SceneHandle sceneHandle) {
		Ref<Scene> scene = validateSceneHandle(sceneHandle);
		if (scene)
			m_ActiveScene = sceneHandle;
	}

	SceneHandle SceneManager::getActiveSceneHandle() const {
		return m_ActiveScene;
	}
	

	Ref<Scene> SceneManager::validateSceneHandle(SceneHandle sceneHandle) {
		if (!sceneHandle) {
			C78_CORE_ERROR("SceneManager::validateSceneHandle: SceneHandle is null!");
			C78_CORE_ASSERT(false);
			return nullptr;
		}
		Ref<AssetManager> assetManager = m_ProjectManager->getActiveProject()->getAssetManager();
		Ref<Asset> asset = assetManager->getAsset(sceneHandle);
		Asset::AssetType type = assetManager->getType(sceneHandle);
		if (!asset || type != Asset::AssetType::Scene) {
			C78_CORE_ERROR("SceneManager::validateSceneHandle: SceneHandle does not correspond to a Scene!");
			C78_CORE_ASSERT(false);
			return nullptr;
		}
		return std::static_pointer_cast<Scene>(asset);
	}

	Ref<Project> SceneManager::getActiveProject() {
		C78_CORE_ASSERT(m_ProjectManager->hasActiveProject(), "SceneManager::getActiveProject: ProjectManager has no active project!");
		return m_ProjectManager->getActiveProject();
	}


	//Editor only
	FilePath SceneManager::fileFromScene(Ref<Scene> scene, Asset::AssetMeta meta) {
		return m_ProjectManager->getActiveProject()->getAssetDirectory() / C78E_ASSETLOC_SCENE / (meta.name + "-" + std::to_string(scene->m_AssetHandle) + C78E_FILE_EXT_SCENE);
	}

}