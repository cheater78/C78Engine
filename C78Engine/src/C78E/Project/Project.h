#pragma once

#include <C78E/Scene/Scene.h>
#include <C78E/Assets/Asset/Asset.h>

#include <C78E/Assets/AssetManager.h>

namespace C78E {

	using ProjectHandle = UUID;

	struct ProjectConfig {
		std::string name = "Untitled Project";

		AssetHandle startScene;

		FilePath assetDirectory;
		FilePath assetRegistryPath; // Relative to AssetDirectory
		FilePath scriptModulePath;
	};

	class Project : public Asset {
	public:
		static Ref<Project> create();
		static Ref<Project> load(const FilePath& path);
		static bool saveActive(const FilePath& path);

		static Ref<Project> getActive() { return s_ActiveProject; }

		static void close() { s_ActiveProject = nullptr; }

		static const FilePath& getActiveProjectDirectory() {
			C78_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->getProjectDirectory();
		}

		static FilePath getActiveAssetDirectory() {
			C78_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->getAssetDirectory();
		}

		static FilePath getActiveAssetRegistryPath() {
			C78_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->getAssetRegistryPath();
		}

		// TODO(Yan): move to asset manager when we have one
		static FilePath getActiveAssetFileSystemPath(const FilePath& path) {
			C78_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->getAssetFileSystemPath(path);
		}

	public:
		const FilePath& getProjectDirectory() { return m_ProjectDirectory; }
		FilePath getAssetDirectory() { return getProjectDirectory() / s_ActiveProject->m_Config.assetDirectory; }
		FilePath getAssetRegistryPath() { return getAssetDirectory() / s_ActiveProject->m_Config.assetRegistryPath; }
		// TODO(Yan): move to asset manager when we have one
		FilePath getAssetFileSystemPath(const FilePath& path) { return getAssetDirectory() / path; }

		FilePath getAssetAbsolutePath(const FilePath& path);
		
		ProjectConfig& getConfig() { return m_Config; }
		Ref<AssetManagerBase> getAssetManager() { return m_AssetManager; }
		Ref<RuntimeAssetManager> getRuntimeAssetManager() { return std::static_pointer_cast<RuntimeAssetManager>(m_AssetManager); }
		Ref<EditorAssetManager> getEditorAssetManager() { return std::static_pointer_cast<EditorAssetManager>(m_AssetManager); }
	
	public:
		//TODO: does this make sense? - Projects being Assets as well?!
		virtual AssetType getType() { return Asset::AssetType::Project; };

	private:
		inline static Ref<Project> s_ActiveProject;

	private:
		ProjectConfig m_Config;
		FilePath m_ProjectDirectory;
		Ref<AssetManagerBase> m_AssetManager;

	};

}