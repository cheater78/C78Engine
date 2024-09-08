#include "AssetManagerUI.h"

namespace C78Editor::GUI {

	AssetManagerUI::AssetManagerUI(C78E::Ref<C78E::ProjectManager> projectManager)
		: m_ProjectManager{ projectManager }, m_CreateAssetPanel(projectManager) {
	}

	AssetManagerUI::~AssetManagerUI() { }

	void AssetManagerUI::onImGuiRender() {
		if (auto assetManager = m_ProjectManager->getEditorAssetManager()) {

			ImGui::Begin("Asset Manager");
			m_CreateAssetPanel.onImGuiRender();
			ImGui::Spacing();
			ImGui::Separator();
			for (auto entry : assetManager->getAssetRegistry()) {
				C78E::AssetHandle handle = entry.first;
				C78E::Asset::AssetMeta meta = entry.second;
				ImGui::SeparatorText((C78E::AssetHandle::encodeToString(handle) + ", " + meta.name).c_str());
				ImGui::Spacing();
				ImGui::Text(meta.name.c_str()); ImGui::SameLine();; ImGui::Text(C78E::Asset::assetTypeToString(meta.type).c_str());
				ImGui::Text(meta.fileSource.string().c_str());
				ImGui::Text(std::to_string(handle).c_str());
				ImGui::Spacing();
				ImGui::Separator();
			}

			ImGui::End();
		}
	}
	

}


