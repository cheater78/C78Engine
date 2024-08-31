#include "SceneManagerUI.h"

namespace C78Editor::GUI {

    SceneManagerUI::SceneManagerUI(C78E::Ref<C78E::SceneManager> sceneManager)
        : m_SceneManager{ sceneManager } {
    }

    SceneManagerUI::~SceneManagerUI() { }

    void C78Editor::GUI::SceneManagerUI::onImGuiRender() {
        ImGui::Begin("SceneManager");
        
        if (m_SceneManager.expired()) {
            ImGui::End();
            return;
        }
        auto sceneManager = m_SceneManager.lock();
        auto projectManager = sceneManager->getProjectManager();
        if (!projectManager->hasActiveProject()) {
            ImGui::End();
            return;
        }
        C78E::Ref<C78E::Project> project = projectManager->getActiveProject();
        C78E::Ref<C78E::EditorAssetManager> assetManager = project->getEditorAssetManager();
        C78E::AssetRegistry assetRegistry = assetManager->getAssetRegistry();


        

        if (ImGui::Button("Create Unnamed Scene"))
            sceneManager->createScene();


        if (sceneManager->hasActiveScene()) {
            ImGui::SameLine();
            if (ImGui::Button("Save")) sceneManager->saveScene();
            ImGui::SameLine();
            if (ImGui::Button("Close")) sceneManager->setActiveSceneHandle(0);

            C78E::SceneHandle sceneHandle = sceneManager->getActiveSceneHandle();
            ImGui::Text(("Current Scene: " + assetManager->getMeta(sceneHandle).name).c_str());
            
        }


        for (C78E::AssetRegistryEntry entry : assetRegistry) {
            C78E::AssetHandle handle = entry.first;
            C78E::Asset::AssetMeta meta = entry.second;

            if (meta.type == C78E::Asset::AssetType::Scene) {
                std::string infoLine = meta.name + " (" + std::to_string(handle) + ")";
                if (ImGui::Button(infoLine.c_str())) {
                    sceneManager->setActiveSceneHandle(handle);
                }
                ImGui::SameLine();
                if (ImGui::Button(("MakeStartScene##" + std::to_string(handle)).c_str())) {
                    projectManager->getActiveProject()->getConfig().startScene = handle;
                }
            }

        }
        ImGui::End();

	}

}
