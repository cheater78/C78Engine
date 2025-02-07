#pragma once

#include <C78E.h>

namespace C78Editor::GUI {

	class AssetManagerUI {
	public:
		AssetManagerUI(C78E::Ref<C78E::ProjectManager> projectManager);
		AssetManagerUI(const AssetManagerUI& other) = delete;
		~AssetManagerUI();

		void onImGuiRender();
	public:
		static void drawCreateNewAsset(C78E::Ref<C78E::ProjectManager> projectManager, C78E::Ref<C78E::EditorAssetManager::AssetDescriptor>& createAssetDescriptor);

		static void drawAssetEditPreview(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Asset::Type type, C78E::AssetHandle& handle, const std::string& label = "");
		static void drawAssetPreview(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Ref<C78E::Asset> asset, const std::string& label = "");
		static void drawAssetEdit(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Asset::Type type, C78E::AssetHandle& handle, const std::string& label);
	private:
		C78E::Ref<C78E::ProjectManager> m_ProjectManager;

		C78E::Ref<C78E::EditorAssetManager::AssetDescriptor> m_CreateAssetDescriptor = nullptr;
	};

}
