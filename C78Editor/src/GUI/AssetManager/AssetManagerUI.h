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

		static void drawAssetEditPreview(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Asset::AssetType type, C78E::AssetHandle& handle, const std::string& label = "");
		static void drawAssetPreview(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Ref<C78E::Asset> asset, const std::string& label = "");
		static void drawAssetEdit(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Asset::AssetType type, C78E::AssetHandle& handle, const std::string& label);
	private:
		C78E::Ref<C78E::ProjectManager> m_ProjectManager;

	private:
		struct CreateAssetPanel {
		public:
			CreateAssetPanel(C78E::Ref<C78E::ProjectManager> projectManager) 
				: m_ProjectManager(projectManager) {
			}

			void reset() {
				assetName.setContent("Unnamed Asset");
				fileSource.setContent("/");
				uuid.setContent(std::to_string(C78E::UUID()));
				assetType.setSelectionIndex(0);

			}

			void onImGuiRender() {
				if (auto assetManager = m_ProjectManager->getEditorAssetManager()) {
					const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

					ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
					float lineHeight = GImGui->Font->FontSize * 6.f + GImGui->Style.FramePadding.y * 2.0f;
					bool open = ImGui::TreeNodeEx("##CreateAsset", treeNodeFlags, "Create Asset");
					ImGui::PopStyleVar();
					ImGui::SameLine(contentRegionAvailable.x - lineHeight * 1.0f);
					if (ImGui::Button("Defaults")) {
						reset();
						if (open)
							ImGui::TreePop();
						return;
					}
					ImGui::SameLine();
					if (ImGui::Button("Create")) {
						const std::string assetNameStr = assetName.getContent();
						const std::string fileSourceStr = fileSource.getContent();
						const std::string uuidStr = uuid.getContent();

						C78E::AssetHandle handle = C78E::AssetHandle::invalid();
						C78E::Asset::AssetMeta meta = C78E::Asset::c_NullAssetMeta;

						if (C78E::FileSystem::exists(fileSourceStr)) {
							meta.name = assetName.getContent();
							meta.fileSource = fileSourceStr;
							meta.type = assetType.getSelected();
							if (C78E::AssetHandle::decodesToValidUUID(uuidStr)) {
								handle = C78E::AssetHandle::decodeFromString(uuidStr);
								assetManager->importAsset(fileSourceStr, meta, handle);
							}
							else {
								C78_EDITOR_ERROR("AssetManagerUI::CreateAssetPanel::onImGuiRender: provided UUID is invalid!");
							}
						}
						else {
							C78_EDITOR_ERROR("AssetManagerUI::CreateAssetPanel::onImGuiRender: provided Source File does not exist!");
						}
					}

					if (open) {
						ImGui::Spacing();
						assetName.show();
						ImGui::Spacing();
						assetType.show();
						ImGui::Spacing();
						fileSource.show();
						ImGui::SameLine();
						fileSourcePicker.show();
						ImGui::Spacing();
						uuid.show();
						ImGui::Spacing();

						ImGui::TreePop();
					}
				}
			}
		public:
			C78E::GUI::TextInput assetName{ "Name", "Unnamed Asset", 64 };
			C78E::GUI::TextInput fileSource{ "FileSource", "/", 254 };
			C78E::GUI::TextButton fileSourcePicker{ "...",
				[this](void) -> void {
					C78_EDITOR_WARN("AssetManagerUI::CreateAssetPanel::fileSourcePicker::onClick: File filter hard coded to .png!");
					fileSource.setContent(C78E::FileDialogs::openFile("Texture\0*.png\0Texture\0*.jpg\0Texture\0*.jpeg\0Font\0*.ttf\0Model\0*.obj\0",fileSource.getContent()).string()); // TODO Asset File Filter List
					assetType.setSelected(C78E::Asset::fileToAssetType(fileSource.getContent()));
				}
			};
			C78E::GUI::TextInput uuid{ "UUID", std::to_string(C78E::UUID()), 32 + (8 - 1) + 2 + 4 + 1 };
			C78E::GUI::ComboInput<C78E::Asset::AssetType> assetType{
				[](C78E::Asset::AssetType type) -> std::string {
					return C78E::Asset::assetTypeToString(type).substr(std::string("AssetType::").size());
				},
				"AssetType",
				C78E::Asset::getAllAssetTypes(),
				0
			};
		private:
			C78E::Ref<C78E::ProjectManager> m_ProjectManager;
		};
		CreateAssetPanel m_CreateAssetPanel;
	};

}
