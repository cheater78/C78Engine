#include "AssetManager.h"

namespace C78Editor {

	bool AssetManager::s_ShowAssetManager = true;

	AssetManager::AssetType AssetManager::s_AssetManagerActiveTab = Shader;

	char AssetManager::s_NewAssetFileInput[s_MaxTextInputSize] = { 0 };
	size_t AssetManager::s_NewAssetFileInputBufferSize = s_MaxTextInputSize;
	char AssetManager::s_NewAssetNameInput[s_MaxTextInputSize] = { 0 };
	size_t AssetManager::s_NewAssetNameInputBufferSize = s_MaxTextInputSize;

	static constexpr ImGuiTabBarFlags s_AssetTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_NoClip | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths;

	void AssetManager::init() {

	}

	void AssetManager::onImGuiRender() {
		if (!ImGui::Begin("AssetManager", &s_ShowAssetManager)) { ImGui::End(); return; }

		ImGui::BeginTabBar("AssetTypeTabBar");

		if (ImGui::TabItemButton("Shader"))
			s_AssetManagerActiveTab = Shader;
		if (ImGui::TabItemButton("Model"))
			s_AssetManagerActiveTab = Model;
		if (ImGui::TabItemButton("Mesh"))
			s_AssetManagerActiveTab = Mesh;
		if (ImGui::TabItemButton("Material"))
			s_AssetManagerActiveTab = Material;
		if (ImGui::TabItemButton("Texture"))
			s_AssetManagerActiveTab = Texture;

		ImGui::EndTabBar();

		//Add Asset
		{
			ImGui::Text("Add Asset:");
			ImGui::InputText("File", s_NewAssetFileInput, s_NewAssetFileInputBufferSize);
			ImGui::SameLine();
			ImGui::InputText("Name", s_NewAssetNameInput, s_NewAssetNameInputBufferSize);
			ImGui::SameLine();
			if (ImGui::Button("Browse")) {
				C78_CORE_WARN("FileManager missing!");
			}
			ImGui::SameLine();
			if (ImGui::Button("Add")) {
				C78_CORE_INFO("Adding Asset");
			}
		}


		ImGui::BeginTable("AssetTable", 4, s_AssetTableFlags);

		ImGui::TableNextRow();
		{
			ImGui::TableNextColumn();
			// Load
			ImGui::Text("Load");
			//--------------------------------------------
			ImGui::TableNextColumn();
			// Name
			ImGui::Text("Name");
			//--------------------------------------------
			ImGui::TableNextColumn();
			// Source
			ImGui::Text("Source");
			//--------------------------------------------
			ImGui::TableNextColumn();
			// Remove
			ImGui::Text("Remove");
			//--------------------------------------------
		}


		if (s_AssetManagerActiveTab == Shader)
			for (std::string name : C78E::AssetManager::()) {
				ImGui::TableNextRow();
				{
					ImGui::TableNextColumn();
					// Loaded
					bool loaded = C78E::AssetManager::getShaderAsset(name).valid();
					if (ImGui::Checkbox(name.c_str(), &loaded)) {
						C78E::AssetManager::loadShader(name);
					}
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Name
					ImGui::Text(name.c_str());
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Source
					ImGui::Text(C78E::AssetManager::getShaderAsset(name).getSource().c_str());
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Remove
					if (ImGui::Button(("Remove" + name).c_str())) {
						C78E::AssetManager::remShader(name);
					}
					//--------------------------------------------
				}
			}
		else if (s_AssetManagerActiveTab == Model)
			for (std::string name : C78E::AssetManager::getAllModel()) {
				ImGui::TableNextRow();
				{
					ImGui::TableNextColumn();
					// Loaded
					//ImGui::Checkbox()
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Name
					ImGui::Text(name.c_str());
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Source
					ImGui::Text(C78E::AssetManager::getModelAsset(name).getSource().c_str());
					//--------------------------------------------
				}
			}
		else if (s_AssetManagerActiveTab == Mesh)
			for (std::string name : C78E::AssetManager::getAllMesh()) {
				ImGui::TableNextRow();
				{
					ImGui::TableNextColumn();
					// Loaded
					//ImGui::Checkbox()
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Name
					ImGui::Text(name.c_str());
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Source
					ImGui::Text(C78E::AssetManager::getMeshAsset(name).getSource().c_str());
					//--------------------------------------------
				}
			}
		else if (s_AssetManagerActiveTab == Material)
			for (std::string name : C78E::AssetManager::getAllMaterial()) {
				ImGui::TableNextRow();
				{
					ImGui::TableNextColumn();
					// Loaded
					//ImGui::Checkbox()
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Name
					ImGui::Text(name.c_str());
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Source
					ImGui::Text(C78E::AssetManager::getMaterialAsset(name).getSource().c_str());
					//--------------------------------------------
				}
			}
		else if (s_AssetManagerActiveTab == Texture)
			for (std::string name : C78E::AssetManager::getAllTexture2D()) {
				ImGui::TableNextRow();
				{
					ImGui::TableNextColumn();
					// Loaded
					//ImGui::Checkbox()
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Name
					ImGui::Text(name.c_str());
					//--------------------------------------------
					ImGui::TableNextColumn();
					// Source
					ImGui::Text(C78E::AssetManager::getTexture2DAsset(name).getSource().c_str());
					//--------------------------------------------
				}
			}

		ImGui::EndTable();

		ImGui::End();
	}

}