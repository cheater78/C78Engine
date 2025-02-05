#pragma once
#include <C78E/Asset/Scene/Entity/Component/Components.h>
#include <C78E/Asset/Scene/Entity/Entity.h>
#include <C78E/Asset/Scene/Entity/ScriptableEntity.h>
#include <C78E/Asset/Scene/Scene.h>
#include <C78E/Asset/AssetManager.h>

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

	class SceneImporter : public EditorAssetManager::Importer {
	public:
		struct SceneMeta : public EditorAssetManager::Meta {
			SceneMeta() = default;
			SceneMeta(const SceneMeta& other) = default;
			SceneMeta(SceneMeta&& other) noexcept = default;
			SceneMeta& operator=(const SceneMeta& other) = default;
			SceneMeta& operator=(SceneMeta&& other) noexcept = default;
			virtual ~SceneMeta() = default;

			// Scene can have all attributes inside its file, no need for specialization
			static void serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo);
			static void deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo);
		};
	public:
		SceneImporter(const FilePath& assetDirectory);
		virtual ~SceneImporter();

		virtual Ref<EditorAssetManager::ImportedAssetGroup> import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) override;
		static bool compatible(const FilePath& assetFile);
	};

	class SceneSerializer {
	public:
	public:
		static Ref<Scene> importScene(AssetHandle handle, const FilePath& filepath);
		static bool exportScene(Ref<Scene> scene, const FilePath& filepath);

		//Scene De-/Serialization
		static bool importEditorScene(Ref<Scene> scene, const std::string& serializedEditorScene);
		static bool importRuntimeScene(Ref<Scene> scene, Ref<ScopedBuffer> serializedRuntimeScene);

		static bool exportEditorScene(Ref<Scene> scene, std::string& serializedEditorScene);
		static bool exportRuntimeScene(Ref<Scene> scene, Ref<ScopedBuffer> serializedRuntimeScene);

	private:
		//Entity De-/Serialization
		static bool importEditorEntity(Ref<Scene> scene, Entity* entity, const YAML::Node& serializedEditorEntity);
		static bool importRuntimeEntity(Ref<Scene> scene, Entity* entity, Ref<ScopedBuffer> serializedRuntimeEnity);

		static bool exportEditorEntity(Ref<Scene> scene, Entity* entity, YAML::Emitter& serializedEditorEntity);
		static bool exportRuntimeEntity(Ref<Scene> scene, Entity* entity, Ref<ScopedBuffer> serializedRuntimeEnity);
	};

}
