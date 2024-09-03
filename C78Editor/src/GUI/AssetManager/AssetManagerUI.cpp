#include "AssetManagerUI.h"

namespace C78Editor::GUI {

	AssetManagerUI::AssetManagerUI(C78E::Ref<C78E::ProjectManager> projectManager)
		: m_ProjectManager{ projectManager } {
	}

	AssetManagerUI::~AssetManagerUI() { }

	void AssetManagerUI::onImGuiRender() {
		if (auto assetManager = getAssetManager()) {

			ImGui::Begin("Asset Manager");
			m_CreateAssetPanel.onImGuiRender();
			ImGui::Spacing();
			ImGui::Separator();
			for (auto entry : assetManager->getAssetRegistry()) {
				C78E::AssetHandle handle = entry.first;
				C78E::Asset::AssetMeta meta = entry.second;
				ImGui::SeparatorText((C78E::AssetHandle::toString(handle) + ", " + meta.name).c_str());
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

	C78E::Ref<C78E::EditorAssetManager> AssetManagerUI::getAssetManager() const {
		if (m_ProjectManager->hasActiveProject())
			return m_ProjectManager->getActiveProject()->getEditorAssetManager();
		return nullptr;
	}
	

}


