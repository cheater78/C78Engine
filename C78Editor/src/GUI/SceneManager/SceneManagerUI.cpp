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
            C78E::SceneHandle sceneHandle = sceneManager->getActiveSceneHandle();
            char nameBuffer[64] = { 0 };
            auto& currName = assetManager->getMeta(sceneHandle)->name;
            const size_t currNameSize = currName.size();
            C78E_ASSERT(currNameSize < 65, "");
            std::memcpy(nameBuffer, assetManager->getMeta(sceneHandle)->name.c_str(), currNameSize);

            ImGui::InputText("##SceneName", nameBuffer, 64);
            currName = nameBuffer;

            ImGui::SameLine();
            if (ImGui::Button("Save")) sceneManager->saveScene();
            ImGui::SameLine();
            if (ImGui::Button("Close")) sceneManager->setActiveSceneHandle(C78E::AssetHandle::invalid());
            
        }


        for (C78E::AssetRegistryEntry entry : assetRegistry) {
            C78E::AssetHandle handle = entry.first;
            C78E::Ref<C78E::Asset::Meta> meta = entry.second;

            if (meta->type == C78E::Asset::Type::Scene) {
                std::string infoLine = meta->name + "##(" + std::to_string(handle) + ")";
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
