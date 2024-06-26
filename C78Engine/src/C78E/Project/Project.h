#pragma once

#include <C78E/Renderer/Assets/Scene/Scene.h>
#include <C78E/Renderer/Assets/Asset/Asset.h>

#include <C78E/Renderer/Assets/AssetManager.h>

namespace C78E {

	using ProjectHandle = UUID;

	struct ProjectConfig {
		std::string name = "Untitled Project";

		AssetHandle startScene = 0;

		FilePath assetDirectory; //absolute
		FilePath assetRegistryPath; //relative to AssetDirectory
		FilePath scriptModulePath;
	};

	class Project { // Editor only!
	public:
		// crude ProjectManagement for the EditorRuntime
		static Ref<Project> create(ProjectConfig config = ProjectConfig(), Ref<AssetManager> assetManager = nullptr);
		static Ref<Project> load(const FilePath& projectFile);
		static bool save(Ref<Project> project, const FilePath& projectFile);
	public:
		const FilePath getProjectDirectory() const { return m_ProjectDirectory; }
		const FilePath getAssetDirectory() const { return m_Config.assetDirectory; }
		const FilePath getAssetRegistryPath() const { return getAssetDirectory() / m_Config.assetRegistryPath; }
		
		
		// TODO(Yan): move to asset manager when we have one
		//FilePath getAssetFileSystemPath(const FilePath& path) { return getAssetDirectory() / path; }
		//FilePath getAssetAbsolutePath(const FilePath& path);
		
		ProjectConfig& getConfig() { return m_Config; }
		Ref<AssetManager> getAssetManager() { return m_AssetManager; }
		Ref<RuntimeAssetManager> getRuntimeAssetManager() { return std::static_pointer_cast<RuntimeAssetManager>(m_AssetManager); }
		Ref<EditorAssetManager> getEditorAssetManager() { return std::static_pointer_cast<EditorAssetManager>(m_AssetManager); }

	private:
		ProjectConfig m_Config;
		FilePath m_ProjectDirectory;
		Ref<AssetManager> m_AssetManager;

	};

}