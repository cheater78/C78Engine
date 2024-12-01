#pragma once
#include <C78E/Renderer/Assets/Scene/Scene.h>
#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/Assets/AssetManager.h>

namespace C78E {

	class Project {
	public:
		struct Config {
		public:
			// needed for clang and gcc, bug reported since ever, but not fixed -> Default Constructor needed
			// if one Constructor is defined we need all Args too
			Config(std::string configName = C78E_DEFAULT_PROJECT_NAME, AssetHandle configStartScene = AssetHandle::invalid(), FilePath configAssetDirectory = C78E_ASSETLOCATION, FilePath configAssetRegistryPath = C78E_ASSETLOCATION_REGISTRY, FilePath configScriptModulePath = C78E_ASSETLOCATION_SCRIPTMODULE)
				: name(configName), startScene(configStartScene), assetDirectory(configAssetDirectory), assetRegistryPath(configAssetRegistryPath), scriptModulePath(configScriptModulePath) {}
		public:
			std::string name = C78E_DEFAULT_PROJECT_NAME;
			AssetHandle startScene = AssetHandle::invalid();

			FilePath assetDirectory = C78E_ASSETLOCATION; //relative to ProjectDirectory
			FilePath assetRegistryPath = C78E_ASSETLOCATION_REGISTRY; //relative to AssetDirectory
			FilePath scriptModulePath = C78E_ASSETLOCATION_SCRIPT; //relative to AssetDirectory
			
			void normalize(const FilePath& projectDirectoryPath);
			const FilePath getAssetDirectory(const FilePath& projectDirectoryPath) const { return projectDirectoryPath / assetDirectory; }
			const FilePath getAssetRegistryPath(const FilePath& projectDirectoryPath) const { return getAssetDirectory(projectDirectoryPath) / assetRegistryPath; }
			const FilePath getScriptModulePath(const FilePath& projectDirectoryPath) const { return getAssetDirectory(projectDirectoryPath) / scriptModulePath; }
			const FilePath getProjectFilePath(const FilePath& projectDirectoryPath) const { return projectDirectoryPath / (name + C78E_FILE_EXT_PROJECT); }
		};
	public:
		static Ref<Project> create(const FilePath& projectDirectoryPath, Config config = Config(), Ref<AssetManager> assetManager = nullptr);
		static Ref<Project> load(const FilePath& projectFile); // projectFile must be absolute
		static bool save(Ref<Project> project, const FilePath& projectFile = {});// projectFile must be absolute
	public:
		const FilePath getProjectDirectory() const { return m_ProjectDirectory; }
		const FilePath getAssetDirectory() const { return m_Config.getAssetDirectory(getProjectDirectory()); }
		const FilePath getAssetRegistryPath() const { return m_Config.getAssetRegistryPath(getProjectDirectory()); }
		const FilePath getScriptModulePath() const { return m_Config.getScriptModulePath(getProjectDirectory()); }
		const FilePath getProjectFilePath() const { return m_Config.getProjectFilePath(getProjectDirectory()); }

		Config& getConfig() { return m_Config; }
		Ref<AssetManager> getAssetManager() { return m_AssetManager; }
		Ref<RuntimeAssetManager> getRuntimeAssetManager() { return std::static_pointer_cast<RuntimeAssetManager>(m_AssetManager); }
		Ref<EditorAssetManager> getEditorAssetManager() { return std::static_pointer_cast<EditorAssetManager>(m_AssetManager); }

	private:
		Config m_Config; // relative to ProjectDirectory (System/C78E Instance independent)
		FilePath m_ProjectDirectory; // System/C78E Instance dependent
		Ref<AssetManager> m_AssetManager;

	};

}