#pragma once

#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor::GUI {

	class AssetManagerUI {
	public:
		AssetManagerUI(C78E::Ref<C78E::ProjectManager> projectManager);
		AssetManagerUI(const AssetManagerUI& other) = delete;
		~AssetManagerUI();

		void onImGuiRender();

	private:

		C78E::Ref<C78E::EditorAssetManager> getAssetManager() const;
	private:
		C78E::Ref<C78E::ProjectManager> m_ProjectManager;

	private:
		struct CreateAssetPanel {
		public:
			C78E::GUI::TextInput assetName{"Name", "Unnamed Asset", 64};
			C78E::GUI::TextInput fileSource{"FileSource", "/", 254};
			C78E::GUI::TextInput uuid{"UUID", std::to_string(C78E::UUID()), 32 + (8 - 1) + 2 + 4};
			C78E::GUI::ComboInput<C78E::Asset::AssetType> assetType{
				[](C78E::Asset::AssetType type) -> std::string {
					return C78E::Asset::assetTypeToString(type).substr(std::string("AssetType::").size());
				},
				"AssetType",
				C78E::Asset::getAllAssetTypes(),
				0
			};

			void onImGuiRender() {
				ImGui::Text("Create Asset");
				assetName.show();
				assetType.show();
				fileSource.show();
				uuid.show();

				if (ImGui::Button("Create")) {

				}
			}

		};
		CreateAssetPanel m_CreateAssetPanel{};
	};

}
