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
			C78E::GUI::TextInput assetName{ "Name", "Unnamed Asset", 64 };
			C78E::GUI::TextInput fileSource{"FileSource", "/", 254};
			C78E::GUI::TextInput uuid{"UUID", std::to_string(C78E::UUID()), 32 + (8 - 1) + 2 + 4 + 1};
			C78E::GUI::ComboInput<C78E::Asset::AssetType> assetType{
				[](C78E::Asset::AssetType type) -> std::string {
					return C78E::Asset::assetTypeToString(type).substr(std::string("AssetType::").size());
				},
				"AssetType",
				C78E::Asset::getAllAssetTypes(),
				0
			};

			void reset() {
				assetName.setContent("Unnamed Asset");
				fileSource.setContent("/");
				uuid.setContent(std::to_string(C78E::UUID()));
				assetType.setSelectionIndex(0);

			}

			void onImGuiRender() {
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				float lineHeight = GImGui->Font->FontSize * 6.f + GImGui->Style.FramePadding.y * 2.0f;
				bool open = ImGui::TreeNodeEx("##CreateAsset", treeNodeFlags, "Create Asset");
				ImGui::PopStyleVar();
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 1.0f);
				if (ImGui::Button("Defaults")) {
					reset();
					if(open)
						ImGui::TreePop();
					return;
				}
				ImGui::SameLine();
				if (ImGui::Button("Create")) {

				}

				if (open) {
					ImGui::Spacing();
					assetName.show();
					ImGui::Spacing();
					assetType.show();
					ImGui::Spacing();
					fileSource.show();
					ImGui::Spacing();
					uuid.show();
					ImGui::Spacing();
					
					ImGui::TreePop();
				}
			}

		};
		CreateAssetPanel m_CreateAssetPanel{};
	};

}
