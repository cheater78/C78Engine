#pragma once
#include <C78E.h>
#include <C78Elibs.h>

#include "Tools/EntityInspector.h"
#include "Tools/Viewport.h"

#include "Scene/EditorCamera.h"

#include <C78E/Project/Project.h>



#include "GUI/ProjectManager/ProjectManager.h"
#include <C78E/Core/FileSystem/FileSearcher.h>

namespace C78Editor {

    /*
    * UI System
    * Asset Loader Importer..
    * 
    * EnttInspector
    */

    class EditorLayer : public C78E::Layer {
    public:

        EditorLayer(C78E::Window& window) : Layer("EditorLayer"),
            m_Window(window),
            m_ProjectManager(C78E::createRef<::C78Editor::GUI::ProjectManager>()),
            m_SceneManager(C78E::createRef<C78E::SceneManager>(m_ProjectManager->getManager()))
        { }

        void onAttach() {
        }

        void onDetach() {
            if (m_ProjectManager->getManager()->hasActiveProject()) {
                C78_EDITOR_ASSERT(m_ProjectManager->getManager()->hasActiveProjectFile(), "EditorLayer::onDetach: no ActiveProjectFile in ProjectManager!");
                m_ProjectManager->getManager()->saveProject(); //TODO: fails if no m_ActiveProjectFile in ProjectManager
            }
        }

        void onUpdate(C78E::Timestep dt) override {
            m_LastFrameTime = dt;

            m_ProjectManager->onUpdate(dt);
            
            /*
            m_EditorScene->onViewportResize(m_Window.getWidth(), m_Window.getHeight());
            if (m_MouseCapture) {
                m_Window.setMouseMode(C78E::MouseMode::DISABLED);
                EditorCamera::update(, delta);
            }
            else m_Window.setMouseMode(C78E::MouseMode::NORMAL);
            */
            //Viewport::onUpdate(nullptr);


        }

        void onEvent(C78E::Event& e) override {
            C78E::EventDispatcher dispatcher(e);
            dispatcher.dispatch<C78E::KeyPressedEvent>(BIND_CALLBACK_FN(EditorLayer::onKeyPressed));
            dispatcher.dispatch<C78E::WindowResizeEvent>(BIND_CALLBACK_FN(EditorLayer::onWindowResize));
        }

        bool onKeyPressed(C78E::KeyPressedEvent e) {
            //TODO: HotkeyManager
            //if (m_HotkeyManager->onKeyPressedEvent(e)) return true; //Hotkeys
            return false;
        }

        bool onWindowResize(C78E::WindowResizeEvent e) {
            C78E::RenderCommand::setViewport(0, 0, e.getWidth(), e.getHeight());
            return true;
        }

        void onImGuiRender() override {
            if (!m_active) return;
            ImGui::DockSpaceOverViewport(); // For Editor -> one big root DockingSpace

            { // Menu Bar
                ImGui::BeginMainMenuBar();

                m_ProjectManager->onImGuiMainMenuBar();

                ImGui::EndMainMenuBar();
            }

            {
                m_ProjectManager->onImGuiRender();
                /*
                ImGui::Begin("ProjectManager");
                if (m_ProjectManager->hasActiveProject()) {
                    ImGui::Text(m_ProjectManager->getActiveProject()->getConfig().name.c_str());

                    if (ImGui::Button("Reload Project")) m_ProjectManager->reloadProject();
                    if (ImGui::Button("Save Project"))
                        if(m_ProjectManager->hasActiveProjectFile()) {
                            m_ProjectManager->saveProject();
                        } else {
                            //TODO:
                            //m_ProjectManager->saveProject(projectFile);
                        }
                    if (ImGui::Button("Close Project"))
                        if (m_ProjectManager->hasActiveProject())
                            m_ProjectManager->closeProject();
                }
                else {
                    if (ImGui::Button("Create Project")); // -> Create Dialog
                    if (ImGui::Button("Open Project")); // -> Open Dialog

                    // Editor Config += last Projects(by projectFiles)
                }

                ImGui::End();
                */
            }

            if(m_ProjectManager->getManager()->hasActiveProject()) {
                ImGui::Begin("SceneManager");

                C78E::Ref<C78E::Project> project = m_ProjectManager->getManager()->getActiveProject();
                C78E::Ref<C78E::EditorAssetManager> assetManager = project->getEditorAssetManager();
                C78E::AssetRegistry assetRegistry = assetManager->getAssetRegistry();

                if (m_SceneManager->hasActiveScene()) {
                    C78E::SceneHandle sceneHandle = m_SceneManager->getActiveSceneHandle();

                    ImGui::Text(("Current Scene: " + assetManager->getMeta(sceneHandle).name).c_str());
                    if (ImGui::Button("Save")) m_SceneManager->saveScene();
                }

                static C78E::GUI::TextInput createSceneTI("create Scene", "UnnamedScene");
                static C78E::GUI::TextButton createSceneTB("New Scene:",
                    [this]() {
                        m_SceneManager->createScene(createSceneTI.getContent());
                    }
                );

                createSceneTI.show();
                C78E::GUI::SameLine();
                createSceneTB.show();

                for (C78E::AssetRegistryEntry entry : assetRegistry) {
                    C78E::AssetHandle handle = entry.first;
                    C78E::Asset::AssetMeta meta = entry.second;

                    if (meta.type == C78E::Asset::AssetType::Scene) {
                        std::string infoLine = meta.name + " (" + std::to_string(handle) + ")";
                        if (ImGui::Button(infoLine.c_str())) {
                            m_SceneManager->setActiveSceneHandle(handle);
                        }
                    }

                }
                ImGui::End();
            }

            {
                ImGui::Begin("FrameInfo");
                ImGui::Text(("FPS: " + std::to_string((uint32_t)(1 / m_LastFrameTime.getSeconds()))).c_str());
                ImGui::Text("FrameTime: %.1f ms", m_LastFrameTime.getMilliSeconds());
                ImGui::Text("FrameTime: %.0f us", m_LastFrameTime.getMilliSeconds() * 1000);

                ImGui::End();
            }
        }



    private:
        C78E::Window& m_Window;
        bool m_active = true;
        C78E::Timestep m_LastFrameTime = 0;
        bool m_MouseCapture = false;

        C78E::Ref<C78E::AssetManager> m_EditorAssetManager; //TODO: for Editor GÙI assets -> Functional: EAM and loaf files -> Goal: RuntimeManager with AssetPack

        C78E::Ref<::C78Editor::GUI::ProjectManager> m_ProjectManager = nullptr;
        C78E::Ref<C78E::SceneManager> m_SceneManager;

        //C78E::Ref<C78E::GUI::HotkeyManager> m_HotkeyManager;
    };

}