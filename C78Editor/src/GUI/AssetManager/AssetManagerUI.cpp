#include "AssetManagerUI.h"

namespace C78Editor::GUI {

	AssetManagerUI::AssetManagerUI(C78E::Ref<C78E::EditorAssetManager> assetManager)
		: m_AssetManager{ assetManager } {
	}

	AssetManagerUI::~AssetManagerUI() { }

	void AssetManagerUI::onImGuiRender() {
		if (auto assetManager = m_AssetManager.lock()) {

			ImGui::Begin("Asset Manager");

			ImGui::InputText("AssetName", nullptr, 0);
			ImGui::InputText("FileSource", nullptr, 0);
			ImGui::InputText("UUID", nullptr, 0);

			int icb = 0;
			std::vector<std::string> label(C78E::Asset::c_AssetTypeCount);
			const char* labelptrs[C78E::Asset::c_AssetTypeCount];
			for (uint8_t i = 0; i < C78E::Asset::c_AssetTypeCount; i++) {
				label.push_back(C78E::Asset::assetTypeToString(static_cast<C78E::Asset::AssetType>(i)));
				labelptrs[i] = label.back().c_str();
			}

			

			ImGui::Combo("AssetType", &icb, labelptrs, label.size());


			ImGui::Button("Create Asset");

			ImGui::End();
		}
	}

	void AssetManagerUI::drawAssetPanelForType(C78E::Asset::AssetType assetType) {
	}
	

}


