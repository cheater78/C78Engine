#pragma once

#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor::GUI {

	class AssetManagerUI {
	public:
		AssetManagerUI(C78E::Ref<C78E::EditorAssetManager> assetManager);
		AssetManagerUI(const AssetManagerUI& other) = delete;
		~AssetManagerUI();

		void onImGuiRender();

	private:
		void drawAssetPanelForType(C78E::Asset::AssetType assetType);


	private:
		C78E::WRef<C78E::EditorAssetManager> m_AssetManager;
	};

}
