#include "C78ePCH.h"
#include "Project.h"
#include "ProjectSerializer.h"
#include <C78E/Renderer/Assets/Scene/SceneSerializer.h>
#include <C78E/Renderer/Assets/Scene/Scene.h>

namespace C78E {

	Ref<Project> Project::create(ProjectConfig config, Ref<AssetManager> assetManager) {
		Ref<Project> project = createRef<Project>();
		project->m_Config = config;
		project->m_AssetManager = (assetManager) ? assetManager : createRef<EditorAssetManager>();

		C78_CORE_ASSERT(!project->m_Config.assetDirectory.empty(), "Project::create: AssetDirectory is empty!");
		//TODO: C78_CORE_ASSERT(!m_Config.scriptModulePath.empty(), "Project::create: ScriptModulePath is empty!");
		FileSystem::createDirectoryIfNotPresent(project->m_Config.assetDirectory);

		if (FileSystem::exists(project->getAssetRegistryPath())) //AssetRegistry alr exists -> load it
			project->getEditorAssetManager()->importAssetRegistry(project->getAssetRegistryPath());
		return project;
	}

	Ref<Project> Project::load(const FilePath& path) {
		Ref<Project> project = createRef<Project>();

		ProjectSerializer serializer(project);
		if (serializer.deserializeProject(path)) {
			project->m_ProjectDirectory = path.parent_path();
			Ref<EditorAssetManager> editorAssetManager = createRef<EditorAssetManager>();
			project->m_AssetManager = editorAssetManager;
			editorAssetManager->importAssetRegistry(project->getAssetRegistryPath());

			//Loading Assets
			AssetRegistry assetRegistry = editorAssetManager->getAssetRegistry();

			for (C78E::AssetRegistryEntry entry : assetRegistry) {
				AssetHandle handle = entry.first;
				Asset::AssetMeta meta = entry.second;

				C78_CORE_INFO("Project::load: Asset({}) added: '{}' from {}", Asset::assetTypeToString(meta.type), meta.name, meta.fileSource.string());
				
				if (editorAssetManager->isValid(handle) && !editorAssetManager->isLoaded(handle) && meta.type != None)
					editorAssetManager->getAsset(handle);

			}

			return project;
		}
		return nullptr;
	}

	bool Project::save(Ref<Project> project, const FilePath& projectFile) {
		FileSystem::createDirectoryIfNotPresent(projectFile.parent_path());
		ProjectSerializer serializer(project);
		if (serializer.serializeProject(projectFile)) {
			FileSystem::createDirectoryIfNotPresent(project->getAssetDirectory());
			FileSystem::createDirectoryIfNotPresent(project->getAssetRegistryPath().parent_path());

			project->m_ProjectDirectory = projectFile.parent_path();
			project->getEditorAssetManager()->exportAssetRegistry(project->getAssetRegistryPath());

			//Saving Assets
			Ref<C78E::EditorAssetManager> assetManager = project->getEditorAssetManager();
			AssetRegistry assetRegistry = assetManager->getAssetRegistry();

			for (C78E::AssetRegistryEntry entry : assetRegistry) {
				AssetHandle handle = entry.first;
				Asset::AssetMeta meta = entry.second;

				// Scenes
				if (meta.type == C78E::Asset::AssetType::Scene) {
					C78_CORE_INFO("Saving Scene: '{}' to {}", meta.name, meta.fileSource);
					SceneSerializer serializer(std::static_pointer_cast<C78E::Scene>(assetManager->getAsset(handle)), meta);
					serializer.serialize();
				}

			}

			return true;
		}
		return false;
	}

}