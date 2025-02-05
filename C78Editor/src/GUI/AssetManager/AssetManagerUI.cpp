#include "AssetManagerUI.h"

namespace C78Editor::GUI {

	AssetManagerUI::AssetManagerUI(C78E::Ref<C78E::ProjectManager> projectManager)
		: m_ProjectManager{ projectManager }, m_CreateAssetDescriptor(C78E::createRef<C78E::EditorAssetManager::AssetDescriptor>(C78E::AssetHandle(), C78E::createRef<C78E::EditorAssetManager::Meta>())) {
	}

	AssetManagerUI::~AssetManagerUI() { }

	void drawFont(C78E::Ref<C78E::Font> font) {
		
	}

	void AssetManagerUI::onImGuiRender() {
		if (auto assetManager = m_ProjectManager->getEditorAssetManager()) {

			ImGui::Begin("Asset Manager");

			

			if (auto assetManager = m_ProjectManager->getEditorAssetManager()) {
				const C78E::FilePath assetDirectoryPath = m_ProjectManager->getActiveProject()->getAssetDirectory();
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				float lineHeight = GImGui->Font->FontSize * 6.f + GImGui->Style.FramePadding.y * 2.0f;
				bool open = ImGui::TreeNodeEx("##CreateAsset", treeNodeFlags, "Create Asset");
				ImGui::PopStyleVar();
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 1.0f);
				if (ImGui::Button("Defaults")) {
					m_CreateAssetDescriptor = C78E::createRef<C78E::EditorAssetManager::AssetDescriptor>();
					m_CreateAssetDescriptor->first = C78E::AssetHandle();
					m_CreateAssetDescriptor->second = C78E::createRef<C78E::EditorAssetManager::Meta>();
					if (open)
						ImGui::TreePop();
					return;
				}
				ImGui::SameLine();
				if (ImGui::Button("Create")) {
					if (C78E::FileSystem::exists(m_ProjectManager->getActiveProject()->getAssetDirectory() / m_CreateAssetDescriptor->second->fileSource)) { //check absolute path
						if(m_CreateAssetDescriptor)
							assetManager->importAsset(*m_CreateAssetDescriptor);
						else
							C78E_ERROR("AssetManagerUI::CreateAssetPanel::onImGuiRender: couldnt create, CreateAssetDescriptor was null!");
					}
					else {
						C78E_ERROR("AssetManagerUI::CreateAssetPanel::onImGuiRender: provided Source File does not exist!");
					}
				}

				if (open) {
					ImGui::Spacing();
					C78E::GUI::drawLabeledTextInput<C78E_PROJECT_NAME_MAX_LENGTH>("AssetName", m_CreateAssetDescriptor->second->name);
					ImGui::Spacing();
					C78E::GUI::drawLabeledComboInput<C78E::Asset::Type>("Type", m_CreateAssetDescriptor->second->type, {0, 1, 2, 3, 4, 5, 6, 7});
					ImGui::Spacing();
					C78E::FilePath createAssetFileSourceAbsolute = assetDirectoryPath / m_CreateAssetDescriptor->second->fileSource;
					const bool fileSourceChanged = C78E::GUI::drawLabeledFileInput<C78E_PATH_MAX>("SourceFile", createAssetFileSourceAbsolute, false, C78E::FileSystem::getAssetEntryTypes(), assetDirectoryPath);
					m_CreateAssetDescriptor->second->fileSource = C78E::FileSystem::getRelativePathTo(createAssetFileSourceAbsolute, assetDirectoryPath);
					if(fileSourceChanged) {
						if(m_CreateAssetDescriptor->second->fileSource.has_filename()) {
							C78E::FilePath filename = m_CreateAssetDescriptor->second->fileSource.filename();
							m_CreateAssetDescriptor->second->name = filename.string().substr(0, filename.string().size() - ((filename.has_extension()) ? filename.extension().string().size() : 0));
						}
						m_CreateAssetDescriptor->second->type = C78E::Asset::Type::typeFromFile(m_CreateAssetDescriptor->second->fileSource);
					}
					ImGui::Spacing();
					std::string uuid = C78E::UUID::encodeToString(m_CreateAssetDescriptor->first);
					C78E::GUI::drawLabeledTextInput<46>("AssetHandle", uuid);
					if(C78E::UUID::decodesToValidUUID(uuid))
						m_CreateAssetDescriptor->first = C78E::UUID::decodeFromString(uuid);
					ImGui::Spacing();

					ImGui::TreePop();
				}
			}
			
			ImGui::Spacing();
			ImGui::Separator();
			if (ImGui::Button("Reload Default Assets")) {
				// Reload any reloads all
				assetManager->reloadAsset(C78E::EditorAssetManager::Default::Texture2D_White);
			}

			if (ImGui::BeginTable("##AssetList", 2, ImGuiTableFlags_NoBordersInBodyUntilResize)) {

				for (auto entry : assetManager->getAssetRegistry()) {
					ImGui::TableNextColumn();

					C78E::AssetHandle handle = entry.first;
					C78E::Ref<C78E::EditorAssetManager::Meta> meta = entry.second;
					ImGui::SeparatorText((C78E::AssetHandle::encodeToString(handle) + ", " + meta->name).c_str());
					ImGui::Spacing();
					ImGui::Text(meta->name.c_str()); ImGui::SameLine(); ImGui::Text(C78E::Asset::Type::assetTypeToString(meta->type).c_str());
					ImGui::Text(meta->fileSource.string().c_str());
					ImGui::Text(std::to_string(handle).c_str());
					ImGui::Spacing();
					ImGui::Separator();

					ImGui::TableNextColumn();
					if (auto asset = assetManager->getAsset(handle)) {
						drawAssetPreview(assetManager, asset, meta->name);
					} else {
						ImGui::Text("Failed to load Asset!");
					}
				}



				ImGui::EndTable();
			}

			ImGui::End();
		}
	}

	void AssetManagerUI::drawAssetEditPreview(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Asset::Type type, C78E::AssetHandle& handle, const std::string& label) {
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
		switch ((uint8_t)asset->getType())
		{
		case C78E::Asset::Type::None: 
		{
			ImGui::Text("Asset Type invalid!");
		}
		break;
		case C78E::Asset::Type::Scene:
		{
			ImGui::Text("Scene: TODO");
		}
		break;
		/*
		case C78E::Asset::Type::Model:
		{
			C78E::Ref<C78E::Model> model = std::static_pointer_cast<C78E::Model>(asset);
			C78E::Ref<C78E::Mesh> mesh = model->mesh();
			C78E::Ref<C78E::Material> material = model->material();
			ImGui::Text("Model:");
			drawAssetPreview(assetManager, mesh, "  Mesh:");
			drawAssetPreview(assetManager, material, "  Material:");
		}
		break;
		*/
		case C78E::Asset::Type::Mesh:
		{
			/*
			C78E::Ref<C78E::Mesh> mesh = std::static_pointer_cast<C78E::Mesh>(asset);
			ImGui::Text("Mesh:");
			ImGui::Text(std::string("  Vertecies: " + std::to_string(mesh->getVertexCount())).c_str());
			ImGui::Text(std::string("  Indecies: " + std::to_string(mesh->getIndexCount())).c_str());
			ImGui::Text(std::string("  Color: " + std::to_string(mesh->hasVertexColor())).c_str());
			ImGui::Text(std::string("  Texture: " + std::to_string(mesh->hasVertexTexture())).c_str());
			*/
		}
		break;
		case C78E::Asset::Type::Material:
		{
			C78E::Ref<C78E::Material> material = std::static_pointer_cast<C78E::Material>(asset);
			ImGui::Text("Material:");
			drawAssetEditPreview(assetManager, C78E::Asset::Type::Shader, material->m_Shader, "Shader");
			//
		}
		break;
		case C78E::Asset::Type::Shader:
		{
			ImGui::Text("Shader: TODO");
		}
		break;
		case C78E::Asset::Type::Texture:
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
			ImGui::Image(texture->getRendererID(), screenSize, ImVec2{ 0.f,1.f }, ImVec2{ 1.f,0.f });
		}
		break;
		case C78E::Asset::Type::Font:
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
			ImGui::Image(atlas->getRendererID(), screenSize, ImVec2{ 0.f,1.f }, ImVec2{ 1.f,0.f });
		}
		break;
		default: return;
		}

	}

	void AssetManagerUI::drawAssetEdit(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Asset::Type type, C78E::AssetHandle& handle, const std::string& label) {
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
			if (entryMeta->type == type) {
				if (entryHandle == handle)
					selected = static_cast<int>(comboEntries.size());
				assetofSameType[entryMeta->name] = entryHandle;
				comboEntries.push_back(entryMeta->name.c_str());
			}

		ImGui::Combo("##AssetEditSelect", &selected, comboEntries.data(), static_cast<int>(comboEntries.size()));

		handle = assetofSameType[comboEntries.at(selected)];

		ImGui::Text(("AssetHandle: " + C78E::AssetHandle::encodeToString(handle)).c_str());
		ImGui::Text(("Type: " + C78E::Asset::Type::assetTypeToString(type)).c_str());

		ImGui::Separator();
		ImGui::PopID();
	}
	


}


