#include "AssetManagerUI.h"

namespace C78Editor::GUI {

	AssetManagerUI::AssetManagerUI(C78E::Ref<C78E::ProjectManager> projectManager)
		: m_ProjectManager{ projectManager }, m_CreateAssetPanel(projectManager) {
	}

	AssetManagerUI::~AssetManagerUI() { }

	void drawFont(C78E::Ref<C78E::Font> font) {
		
	}

	void AssetManagerUI::onImGuiRender() {
		if (auto assetManager = m_ProjectManager->getEditorAssetManager()) {

			ImGui::Begin("Asset Manager");
			m_CreateAssetPanel.onImGuiRender();
			ImGui::Spacing();
			ImGui::Separator();


			if (ImGui::BeginTable("##AssetList", 2, ImGuiTableFlags_NoBordersInBodyUntilResize)) {

				for (auto entry : assetManager->getAssetRegistry()) {
					ImGui::TableNextColumn();

					C78E::AssetHandle handle = entry.first;
					C78E::Asset::AssetMeta meta = entry.second;
					ImGui::SeparatorText((C78E::AssetHandle::encodeToString(handle) + ", " + meta.name).c_str());
					ImGui::Spacing();
					ImGui::Text(meta.name.c_str()); ImGui::SameLine(); ImGui::Text(C78E::Asset::assetTypeToString(meta.type).c_str());
					ImGui::Text(meta.fileSource.string().c_str());
					ImGui::Text(std::to_string(handle).c_str());
					ImGui::Spacing();
					ImGui::Separator();

					ImGui::TableNextColumn();
					if (auto asset = assetManager->getAsset(handle)) {
						drawAssetPreview(assetManager, asset, meta.name);
					} else {
						ImGui::Text("Failed to load Asset!");
					}
				}



				ImGui::EndTable();
			}

			ImGui::End();
		}
	}

	void AssetManagerUI::drawAssetEditPreview(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Asset::AssetType type, C78E::AssetHandle& handle, const std::string& label) {
		// edit -> call drawAssetEdit()
		AssetManagerUI::drawAssetEdit(assetManager, type, handle, label);
		// show asset overwiew
		if (handle.isValid()) {
			if (auto asset = assetManager->getAsset(handle)) {
				drawAssetPreview(assetManager, asset, label);
			} else {
				ImGui::Text("Failed to load Asset!");
			}
		}
		
	}

	void AssetManagerUI::drawAssetPreview(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Ref<C78E::Asset> asset, const std::string& label) {
		switch (asset->getType())
		{
		case C78E::Asset::None: 
		{
			ImGui::Text("Asset Type invalid!");
		}
		break;
		case C78E::Asset::Scene:
		{
			ImGui::Text("Scene: TODO");
		}
		break;
		case C78E::Asset::Model:
		{
			C78E::Ref<C78E::Model> model = std::static_pointer_cast<C78E::Model>(asset);
			C78E::Ref<C78E::Mesh> mesh = model->mesh();
			C78E::Ref<C78E::Material> material = model->material();
			ImGui::Text("Model:");
			drawAssetPreview(assetManager, mesh, "  Mesh:");
			drawAssetPreview(assetManager, material, "  Material:");
		}
		break;
		case C78E::Asset::Mesh:
		{
			C78E::Ref<C78E::Mesh> mesh = std::static_pointer_cast<C78E::Mesh>(asset);
			ImGui::Text("Mesh:");
			ImGui::Text(std::string("  Vertecies: " + std::to_string(mesh->getVertexCount())).c_str());
			ImGui::Text(std::string("  Indecies: " + std::to_string(mesh->getIndexCount())).c_str());
			ImGui::Text(std::string("  Color: " + std::to_string(mesh->hasVertexColor())).c_str());
			ImGui::Text(std::string("  Texture: " + std::to_string(mesh->hasVertexTexture())).c_str());
		}
		break;
		case C78E::Asset::Material:
		{
			C78E::Ref<C78E::Material> material = std::static_pointer_cast<C78E::Material>(asset);
			ImGui::Text("Material:");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Shader, material->m_Shader, "Shader");
			ImGui::Text("  Textures:");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Texture2D, material->m_MaterialTextures.ambient, "Ambient");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Texture2D, material->m_MaterialTextures.diffuse, "Diffuse");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Texture2D, material->m_MaterialTextures.specular, "Specular");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Texture2D, material->m_MaterialTextures.specularHighlight, "SpecularHighlight");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Texture2D, material->m_MaterialTextures.bump, "Bump");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Texture2D, material->m_MaterialTextures.displacement, "Displacement");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Texture2D, material->m_MaterialTextures.alpha, "Alpha");
			drawAssetEditPreview(assetManager, C78E::Asset::AssetType::Texture2D, material->m_MaterialTextures.reflection, "Reflection");
		}
		break;
		case C78E::Asset::Shader:
		{
			ImGui::Text("Shader: TODO");
		}
		break;
		case C78E::Asset::Texture2D:
		{
			C78E::Ref<C78E::Texture2D> texture = std::static_pointer_cast<C78E::Texture2D>(asset);

			const ImVec2 atlasSize = { static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()) };
			const ImVec2 screenSize = [atlasSize]() -> ImVec2 {
				ImVec2 size = ImGui::GetContentRegionAvail();
				size.x /= 1;
				size.y = size.x;

				if (atlasSize.x > atlasSize.y)
					size.y *= static_cast<float>(atlasSize.y) / atlasSize.x;
				else
					size.x *= static_cast<float>(atlasSize.x) / atlasSize.y;
				return size;
			}();
			ImGui::Text("Texture2D:");
			ImGui::Image(C78E::GUI::TextureHandle(texture->getRendererID()), screenSize, ImVec2{ 0.f,1.f }, ImVec2{ 1.f,0.f });
		}
		break;
		case C78E::Asset::CubeMap:
		{
			C78E::Ref<C78E::CubeMap> cubeMap = std::static_pointer_cast<C78E::CubeMap>(asset);

			const ImVec2 atlasSize = { static_cast<float>(cubeMap->getSize()), static_cast<float>(cubeMap->getSize()) };
			const ImVec2 screenSize = [atlasSize]() -> ImVec2 {
				ImVec2 size = ImGui::GetContentRegionAvail();
				size.x /= 1;
				size.y = size.x;

				if (atlasSize.x > atlasSize.y)
					size.y *= static_cast<float>(atlasSize.y) / atlasSize.x;
				else
					size.x *= static_cast<float>(atlasSize.x) / atlasSize.y;
				return size;
				}();
			ImGui::Text("CubeMap: TODO");
			//ImGui::Image(C78E::GUI::TextureHandle(cubeMap->getRendererID()), screenSize, ImVec2{ 0.f,1.f }, ImVec2{ 1.f,0.f });
		}
		break;
		case C78E::Asset::Font:
		{
			C78E::Ref<C78E::Font> font = std::static_pointer_cast<C78E::Font>(asset);
			C78E::Ref<C78E::Texture2D> atlas = font->getAtlasTexture();

			const ImVec2 atlasSize = { static_cast<float>(atlas->getWidth()), static_cast<float>(atlas->getHeight()) };
			const ImVec2 screenSize = [atlasSize]() -> ImVec2 {
				ImVec2 size = ImGui::GetContentRegionAvail();
				size.x /= 1;
				size.y = size.x;

				if (atlasSize.x > atlasSize.y)
					size.y *= static_cast<float>(atlasSize.y) / atlasSize.x;
				else
					size.x *= static_cast<float>(atlasSize.x) / atlasSize.y;
				return size;
			}();
			ImGui::Text("Font:");
			ImGui::Image(C78E::GUI::TextureHandle(atlas->getRendererID()), screenSize, ImVec2{ 0.f,1.f }, ImVec2{ 1.f,0.f });
		}
		break;
		default: return;
		}

	}

	void AssetManagerUI::drawAssetEdit(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Asset::AssetType type, C78E::AssetHandle& handle, const std::string& label) {
		ImGui::PushID(label.c_str());
		if (!label.empty()) {
			ImGui::SeparatorText(label.c_str());
		}
		else ImGui::Separator();

		std::unordered_map<std::string, C78E::AssetHandle> assetofSameType{};
		int selected = 0;
		std::vector<const char*> comboEntries;

		const char* none = "None";
		assetofSameType[none] = C78E::AssetHandle::invalid();
		comboEntries.push_back(none);

		for (auto& [entryHandle, entryMeta] : assetManager->getAssetRegistry())
			if (entryMeta.type == type) {
				if (entryHandle == handle)
					selected = static_cast<int>(comboEntries.size());
				assetofSameType[entryMeta.name] = entryHandle;
				comboEntries.push_back(entryMeta.name.c_str());
			}

		ImGui::Combo("##AssetEditSelect", &selected, comboEntries.data(), static_cast<int>(comboEntries.size()));

		handle = assetofSameType[comboEntries.at(selected)];

		ImGui::Text(("AssetHandle: " + C78E::AssetHandle::encodeToString(handle)).c_str());
		ImGui::Text(("AssetType: " + C78E::Asset::assetTypeToString(type)).c_str());

		ImGui::Separator();
		ImGui::PopID();
	}
	


}


