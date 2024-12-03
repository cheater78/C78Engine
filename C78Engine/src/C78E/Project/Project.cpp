#include "C78EPCH.h"
#include "Project.h"

#include <C78E/Project/ProjectSerializer.h>
#include <C78E/Renderer/Assets/Scene/SceneSerializer.h>
#include <C78E/Renderer/Assets/Scene/Scene.h>

namespace C78E {

	void Project::Config::normalize(const FilePath& projectDirectoryPath) {
		assetDirectory = FileSystem::normalizePath(FileSystem::getRelativePathTo(assetDirectory, projectDirectoryPath));
		assetRegistryPath = FileSystem::normalizePath(FileSystem::getRelativePathTo(assetRegistryPath, projectDirectoryPath / assetDirectory));
		scriptModulePath = FileSystem::normalizePath(FileSystem::getRelativePathTo(scriptModulePath, projectDirectoryPath / assetDirectory));
	}

	/*
	* Creates a Project, ProjectName and AssetRegistryFile must be set(are per Default)
	* an existinf AssetManager, including its Assets, can be provided
	*/
	Ref<Project> Project::create(const FilePath& projectDirectoryPath, Config config, Ref<AssetManager> assetManager) {
		C78E_CORE_VALIDATE(projectDirectoryPath.is_absolute(), return nullptr, "Project::create: Provided projectDirectoryPath must be absolute!");
		C78E_CORE_VALIDATE(FileSystem::isDirectory(projectDirectoryPath), return nullptr, "Project::create: Provided projectDirectoryPath must be a Directory!");
		Ref<Project> project = createRef<Project>();
		project->m_Config = config;
		project->m_ProjectDirectory = projectDirectoryPath;
		project->m_AssetManager = (assetManager) ? assetManager : createRef<EditorAssetManager>(project->getAssetDirectory());
		C78E_CORE_VALIDATE(!project->m_Config.name.empty(), return nullptr, "Project::create: Project can't be created with an empty Name!");
		C78E_CORE_VALIDATE(!project->m_Config.assetRegistryPath.empty(), return nullptr, "Project::create: Project can't be created with an empty AssetRegistry FilePath!");
		// TODO: check existing load existing
		return project;
	}

	/*
	* Loads a Project, provided its ProjectFile, FilePath must be absolute,
	* loads the Projects AssetRegistry, if non-existent creates a new one,
	* loads all Assets specified by the AssetRegistry, skips corrupted Entries and removes them from the AssetRegistry
	*/
	Ref<Project> Project::load(const FilePath& projectFile) {
		C78E_CORE_VALIDATE(projectFile.is_absolute(), return nullptr, "Project::load: Provided ProjectFile must be absolute!");
		C78E_CORE_VALIDATE(FileSystem::isFile(projectFile), return nullptr, "Project::load: Provided ProjectFile must be a File!");
		Ref<Project> project = ProjectSerializer::importProject(projectFile);
		C78E_CORE_VALIDATE(project, return nullptr, "Project::load: Failed to deserialize Project!\n    File: {}", projectFile);
		
		project->m_ProjectDirectory = projectFile.parent_path();
		Ref<EditorAssetManager> editorAssetManager = createRef<EditorAssetManager>(project->getAssetDirectory());
		project->m_AssetManager = editorAssetManager;

		if(!FileSystem::exists(project->getAssetRegistryPath())) {
			C78E_CORE_WARN("Project::load: Creating AssetRegistry at: {}", project->getAssetRegistryPath());
			editorAssetManager->exportAssetRegistry(project->getAssetRegistryPath());
			return project;
		}
		editorAssetManager->importAssetRegistry(project->getAssetRegistryPath());

		//Loading Assets
		AssetRegistry assetRegistry = editorAssetManager->getAssetRegistry();
		for (C78E::AssetRegistryEntry entry : assetRegistry) {
			AssetHandle handle = entry.first;
			Ref<Asset::Meta> meta = entry.second;

			if(!editorAssetManager->isValid(handle) || meta->type == Asset::Type::None) {
				C78E_CORE_WARN("Project::load: Asset '{}' of Type {} from File {} is invalid!", meta->name, Asset::Type::assetTypeToString(meta->type), meta->fileSource.string());
				editorAssetManager->removeAsset(handle, false); // AssetRegistry Entry corrupted -> remove, but not from Disk
			} else if(!editorAssetManager->isLoaded(handle)) {
				editorAssetManager->getAsset(handle);
				C78E_CORE_INFO("Project::load: Asset '{}' of Type {} from File {} was loaded!", meta->name, Asset::Type::assetTypeToString(meta->type), meta->fileSource.string());
			} else {
				C78E_CORE_TRACE("Project::load: Asset '{}' of Type {} from File {} already loaded!", meta->name, Asset::Type::assetTypeToString(meta->type), meta->fileSource.string());
			}
		}
		return project;
	}

	/*
	* Saves a Project, requires a ProjectFile on first save
	* TODO: ProjectFiles not following the getProjectFilePath() naming are possible, but something to reconsider!
	*/
	bool Project::save(Ref<Project> project, const FilePath& projectFile) {
		if(!projectFile.empty()) {
			C78E_CORE_VALIDATE(projectFile.is_absolute(), return false, "Project::save: Provided ProjectFile is not an absolute Path!");
			C78E_CORE_VALIDATE(projectFile.parent_path() == project->m_ProjectDirectory || project->m_ProjectDirectory.empty(), return false, "Project::save: Changing the ProjectDirectory after first time setup is not supported(would require moving the whole folder, w/ all assets,..)!");
			project->m_ProjectDirectory = projectFile.parent_path();
		} else {
			C78E_CORE_VALIDATE(project->m_ProjectDirectory.is_absolute(), return false, "Project::save: ProjectDirectory is not an absolute Path!(No ProjectFile provided!)");
		}
		project->m_Config.normalize(project->m_ProjectDirectory); // probably not nessecary
		
		C78E_CORE_VALIDATE(ProjectSerializer::exportProject(project, projectFile), return false, "Project::save: Failed to serialize Project!\n    File: {}", projectFile);

		//Export Assets
		Ref<C78E::EditorAssetManager> assetManager = project->getEditorAssetManager();
		const AssetRegistry& assetRegistry = assetManager->getAssetRegistry();
		assetManager->exportAssetRegistry(project->getAssetRegistryPath());

		for (C78E::AssetRegistryEntry entry : assetRegistry) {
			AssetHandle handle = entry.first;
			Ref<Asset::Meta> meta = entry.second;

			// Scenes
			if (meta->type == C78E::Asset::Type::Scene) {
				C78E_CORE_VALIDATE(SceneSerializer::exportScene(assetManager->getAssetAs<Scene>(handle), FileSystem::normalizePath(project->getAssetDirectory() / meta->fileSource)), continue, "Project::save: Failed to serialize Scene!\n    File: {}", FileSystem::normalizePath(project->getAssetDirectory() / meta->fileSource));
				C78E_CORE_INFO("Saved Scene: '{}' to {}", meta->name, meta->fileSource);
			}
		}
		return true;
	}

}