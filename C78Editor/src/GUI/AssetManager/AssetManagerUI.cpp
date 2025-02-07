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

			//Create Asset Panel
			drawCreateNewAsset(m_ProjectManager, m_CreateAssetDescriptor);
			
			ImGui::Spacing();
			ImGui::Separator();
			if (ImGui::Button("Reload Default Assets")) {
				// Reload any reloads all
				assetManager->reloadAsset(C78E::EditorAssetManager::Default::Texture2D_White);
			}

			/*
			* Asset List
			*/
			if (ImGui::BeginTable("##AssetList", 2, ImGuiTableFlags_NoBordersInBodyUntilResize)) {
				for (auto entry : assetManager->getAssetRegistry()) {
					const C78E::AssetHandle handle = entry.first;
					C78E::Ref<C78E::EditorAssetManager::Meta> meta = entry.second;

					ImGui::TableNextColumn();
					{
						
						ImGui::SeparatorText((C78E::AssetHandle::encodeToString(handle) + ", " + meta->name).c_str());
						ImGui::Spacing();
						ImGui::Text(meta->name.c_str()); ImGui::SameLine(); ImGui::Text(C78E::Asset::Type::assetTypeToString(meta->type).c_str());
						ImGui::Text(meta->fileSource.string().c_str());
						ImGui::Text(std::to_string(handle).c_str());
						ImGui::Spacing();
						ImGui::Separator();
					}
					ImGui::TableNextColumn();
					{
						if (auto asset = assetManager->getAsset(handle)) {
							drawAssetPreview(assetManager, asset, meta->name);
						}
						else {
							ImGui::Text("Failed to load Asset!");
						}
					}
				}



				ImGui::EndTable();
			}

			ImGui::End();
		}
	}

	void AssetManagerUI::drawCreateNewAsset(C78E::Ref<C78E::ProjectManager> projectManager, C78E::Ref<C78E::EditorAssetManager::AssetDescriptor>& createAssetDescriptor) {
		C78E_VALIDATE(projectManager, "AssetManagerUI::drawCreateAssetPanel: projectManager was null!");
		C78E_VALIDATE(projectManager->hasActiveProject(), "AssetManagerUI::drawCreateAssetPanel: projectManager had no active Project!");
		C78E::Ref<C78E::EditorAssetManager> assetManager = projectManager->getEditorAssetManager();
		const C78E::FilePath absoluteAssetDirectoryPath = projectManager->getActiveProject()->getAssetDirectory();


		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

		float lineHeight = GImGui->Font->FontSize * 6.f;
		bool open = ImGui::TreeNodeEx("##CreateAsset", treeNodeFlags, "Create Asset");
		
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 1.0f);
		if (ImGui::Button("Defaults")) {
			createAssetDescriptor = C78E::createRef<C78E::EditorAssetManager::AssetDescriptor>();
			createAssetDescriptor->first = C78E::AssetHandle();
			createAssetDescriptor->second = C78E::createRef<C78E::EditorAssetManager::Meta>();
		}

		C78E::FilePath absoluteAssetSourceFilePath = absoluteAssetDirectoryPath / createAssetDescriptor->second->fileSource;

		ImGui::SameLine();
		if (ImGui::Button("Create")) {
			if (C78E::FileSystem::exists(absoluteAssetSourceFilePath)) { //check absolute path
				if (createAssetDescriptor)
					assetManager->importAsset(*createAssetDescriptor);
				else
					C78E_ERROR("AssetManagerUI::CreateAssetPanel::onImGuiRender: couldnt create, CreateAssetDescriptor was null!");
			}
			else {
				C78E_ERROR("AssetManagerUI::CreateAssetPanel::onImGuiRender: provided Source File does not exist!");
			}
		}

		ImGui::PopStyleVar();

		if (open) {
			ImGui::Spacing();
			C78E::GUI::drawLabeledTextInput<C78E_PROJECT_NAME_MAX_LENGTH>("AssetName", createAssetDescriptor->second->name);
			ImGui::Spacing();
			C78E::GUI::drawLabeledComboEnumInput<C78E::Asset::Type>("Type", createAssetDescriptor->second->type, C78E::Asset::Type::TYPE_SIZE);
			ImGui::Spacing();

			const bool fileSourceChanged = C78E::GUI::drawLabeledFileInput<C78E_PATH_MAX>("SourceFile", absoluteAssetSourceFilePath, false, C78E::FileSystem::getAssetEntryTypes(), absoluteAssetDirectoryPath);
			if (fileSourceChanged) {
				if (createAssetDescriptor->second->fileSource.has_filename()) {
					C78E::FilePath filename = createAssetDescriptor->second->fileSource.filename();
					createAssetDescriptor->second->name = filename.string().substr(0, filename.string().size() - ((filename.has_extension()) ? filename.extension().string().size() : 0));
				}

				createAssetDescriptor->second->fileSource = C78E::FileSystem::getRelativePathTo(absoluteAssetSourceFilePath, absoluteAssetDirectoryPath);
				createAssetDescriptor->second->type = C78E::Asset::Type::typeFromFile(createAssetDescriptor->second->fileSource);
			}
			ImGui::Spacing();
			std::string uuid = C78E::UUID::encodeToString(createAssetDescriptor->first);
			C78E::GUI::drawLabeledTextInput<46>("AssetHandle", uuid);
			if (C78E::UUID::decodesToValidUUID(uuid))
				createAssetDescriptor->first = C78E::UUID::decodeFromString(uuid);
			ImGui::Spacing();

			ImGui::TreePop();
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

	// drawAssetLink -> show, basic meta info + Button to jump to the Asset in the List 

	void AssetManagerUI::drawAssetPreview(C78E::Ref<C78E::EditorAssetManager> assetManager, C78E::Ref<C78E::Asset> asset, const std::string& label) {
		switch ((uint8_t)asset->getType()) {
		case C78E::Asset::Type::None: return;
		case C78E::Asset::Type::Scene:
		{
			C78E::Ref<C78E::Scene> scene = std::static_pointer_cast<C78E::Scene>(asset);
			ImGui::Text("Scene: TODO");
		}
		return;
		case C78E::Asset::Type::Model:
		{
			C78E::Ref<C78E::Model> model = std::static_pointer_cast<C78E::Model>(asset);
			ImGui::Text("DrawableMeshes: ");
			for (auto& drawableMesh : model->getDrawableMeshes()) {
				ImGui::Text("  DrawableMesh:");
				drawAssetEditPreview(assetManager, C78E::Asset::Type::Mesh, drawableMesh.first, "    Mesh:");
				drawAssetEditPreview(assetManager, C78E::Asset::Type::Material, drawableMesh.second, "    Material:");
			}
		}
		return;
		case C78E::Asset::Type::Mesh:
		{
			C78E::Ref<C78E::Mesh> mesh = std::static_pointer_cast<C78E::Mesh>(asset);
			ImGui::Text("Mesh:");
			ImGui::Text(std::string("  Vertecies: " + std::to_string(mesh->vertexCount())).c_str());
			ImGui::Text(std::string("  Halfedges: " + std::to_string(mesh->halfedgeCount())).c_str());
			ImGui::Text(std::string("  Faces: " + std::to_string(mesh->faceCount())).c_str());
			ImGui::Text(std::string("  FaceHalfedges: " + std::to_string(mesh->faceHalfedgeCount())).c_str());
		}
		return;
		case C78E::Asset::Type::Material:
		{
			C78E::Ref<C78E::Material> material = std::static_pointer_cast<C78E::Material>(asset);
			drawAssetEditPreview(assetManager, C78E::Asset::Type::Material, material->m_Shader, "Material:");
			//
		}
		return;
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


