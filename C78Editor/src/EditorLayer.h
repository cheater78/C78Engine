#pragma once
#include <C78E.h>
#include <C78Elibs.h>

#include "Tools/Viewport.h"

#include "C78E/Renderer/Assets/Asset/AssetImporter.h"

#include <C78E/Project/Project.h>

#include "GUI/Viewport/ViewportUI.h"
#include "GUI/EntityManager/EntityManagerUI.h"
#include "GUI/AssetManager/AssetManagerUI.h"
#include "GUI/SceneManager/SceneManagerUI.h"
#include "GUI/ProjectManager/ProjectManagerUI.h"

namespace C78Editor {

    /*
    * 
    * Sprite for Renderer3D
    * 
    */

    class EditorLayer : public C78E::Layer {
    public:

        EditorLayer(C78E::Window& window) : Layer("EditorLayer"),
            m_Window(window),
            m_ProjectManager(C78E::createRef<C78E::ProjectManager>()),
            m_SceneManager(C78E::createRef<C78E::SceneManager>(m_ProjectManager)),

            m_ProjectManagerUI(C78E::createRef <::C78Editor::GUI::ProjectManagerUI>(m_ProjectManager)),
            m_AssetManagerUI(C78E::createRef <::C78Editor::GUI::AssetManagerUI>(m_ProjectManager)),
            m_SceneManagerUI(C78E::createRef <::C78Editor::GUI::SceneManagerUI>(m_SceneManager)),
            m_EntityManagerUI(C78E::createRef<::C78Editor::GUI::EntityManagerUI>(m_SceneManager)),
            m_ViewportUI(C78E::createRef<::C78Editor::GUI::ViewportUI>(m_ProjectManager))
        { }

        void onAttach() { }

        void onDetach() {
            if (m_ProjectManager->hasActiveProject())
                m_ProjectManager->saveProject();
        }

        void onUpdate(C78E::Timestep dt) override {
            m_LastFrameTime = dt;
            
            if (m_ProjectManager->hasActiveProject()) {
                //RendererManager?
                if (!m_Renderer) {
                    m_Renderer = std::static_pointer_cast<C78E::Renderer>(C78E::createRef<C78E::Rasterizer3D>(m_ProjectManager->getActiveProject()->getEditorAssetManager()));
                }

                if (m_SceneManager->hasActiveScene()) {
                    C78E::Ref<C78E::Scene> scene = m_SceneManager->getActiveScene();
                    /*
                    if (m_MouseCapture) {
                        if (m_Window.getMouseMode() != C78E::MouseMode::DISABLED) {
                            C78E::Input::setMousePosition(m_Window.getWidth() / 2, m_Window.getHeight() / 2);
                            m_Window.setMouseMode(C78E::MouseMode::DISABLED);
                        }
                        C78E::Entity camera = scene->getPrimaryCamera();

                        auto& camTrans = camera.getComponent<C78E::TransformComponent>();

                        static const float MouseSens = 0.1f;
                        float MoveSpeed = 2.f;
                        if (!C78E::Input::isKeyPressed(C78E::Key::LeftShift)) MoveSpeed *= 3;

                        auto& rot = camTrans.Rotation;
                        rot += glm::vec3(0.f, (C78E::Input::getMouseX() - m_Window.getWidth() / 2) * dt.getSeconds() * MouseSens, 0.f);
                        rot += glm::vec3( (C78E::Input::getMouseY() - m_Window.getHeight() / 2) * dt.getSeconds() * MouseSens, 0.f, 0.f);
                        C78E::Input::setMousePosition(m_Window.getWidth()/2, m_Window.getHeight() / 2);

                        const glm::vec3 lookat = glm::normalize(glm::vec3{ sin(rot.y), .0f, cos(rot.y) });
                        const glm::vec3 upDir{ 0.f, 1.f, 0.f };
                        const glm::vec3 rightDir = glm::normalize(glm::cross(lookat, upDir));
                        const glm::vec3 forwardDir = glm::normalize(glm::cross(-rightDir, upDir));
                        auto& transform = camTrans.Translation;
                        if (C78E::Input::isKeyPressed(C78E::Key::W)) { transform += forwardDir * dt.getSeconds() * MoveSpeed; }
                        if (C78E::Input::isKeyPressed(C78E::Key::S)) { transform -= forwardDir * dt.getSeconds() * MoveSpeed; }
                        if (C78E::Input::isKeyPressed(C78E::Key::A)) { transform += rightDir * dt.getSeconds() * MoveSpeed; }
                        if (C78E::Input::isKeyPressed(C78E::Key::D)) { transform -= rightDir * dt.getSeconds() * MoveSpeed; }
                        if (C78E::Input::isKeyPressed(C78E::Key::Space)) { transform += upDir * dt.getSeconds() * MoveSpeed; }
                        if (C78E::Input::isKeyPressed(C78E::Key::LeftControl)) { transform -= upDir * dt.getSeconds() * MoveSpeed; }
                    }
                    else m_Window.setMouseMode(C78E::MouseMode::NORMAL);
                    */

                    //RendererManager?
                    m_ViewportUI->setActiveScene(m_SceneManager->getActiveScene());
                    m_ViewportUI->onUpdate(dt);

                    
                }
            }
            


        }

        void onEvent(C78E::Event& e) override {
            m_ViewportUI->onEvent(e); //TODO: cleanup

            C78E::EventDispatcher dispatcher(e);
            dispatcher.dispatch<C78E::WindowResizeEvent>(BIND_CALLBACK_FN(EditorLayer::onWindowResize));
        }

        bool onWindowResize(C78E::WindowResizeEvent e) {
            C78E::RenderCommand::setViewport(0, 0, e.getWidth(), e.getHeight());
            return true;
        }

        void onImGuiRender() override {
            ImGui::DockSpaceOverViewport(); // For Editor -> one big root DockingSpace

            { // Menu Bar
                ImGui::BeginMainMenuBar();

                m_ProjectManagerUI->onImGuiMainMenuBar();

                ImGui::EndMainMenuBar();
            }

            ImGui::ShowDemoWindow();
            
            m_ProjectManagerUI->onImGuiRender();
            m_AssetManagerUI->onImGuiRender();
            m_SceneManagerUI->onImGuiRender();
            m_EntityManagerUI->onImGuiRender();
            m_ViewportUI->onImGuiRender();



            {
                ImGui::Begin("FrameInfo");
                ImGui::Text(("FPS: " + std::to_string((uint32_t)(1 / m_LastFrameTime.getSeconds()))).c_str());
                ImGui::Text("FrameTime: %.1f ms", m_LastFrameTime.getMilliSeconds());
                ImGui::Text("FrameTime: %.0f us", m_LastFrameTime.getMilliSeconds() * 1000);

                if (m_Renderer)
                    ImGui::Text("Fragments: %1i x %1i = %1i px", m_Renderer->getTargetTexture()->getWidth(), m_Renderer->getTargetTexture()->getHeight(), m_Renderer->getTargetTexture()->getWidth() * m_Renderer->getTargetTexture()->getHeight());
                else
                    ImGui::Text("No Renderer");

                ImGui::Text("ImGui Win: [active: %1i, visible: %1i]", ImGui::GetIO().MetricsActiveWindows, ImGui::GetIO().MetricsRenderWindows);


                if(m_ProjectManager->hasActiveProject() && ImGui::Button("Reload Default Shaders")) {
                    // Reload any reloads all
                    m_ProjectManager->getEditorAssetManager()->reloadAsset(C78E::EditorAssetManager::Default::Shader_SpriteRenderComponent);
                }

                ImGui::End();
            }

        }



    private:
        C78E::Window& m_Window;
        C78E::Timestep m_LastFrameTime = 0;

        //C78E::Ref<C78E::AssetManager> m_EditorAssetManager; //TODO: for Editor GÙI assets -> Functional: EAM and loaf files -> Goal: RuntimeManager with AssetPack

        C78E::Ref<C78E::Scene> m_EditorScene = nullptr;

        C78E::Ref<C78E::ProjectManager> m_ProjectManager = nullptr;
        C78E::Ref<C78E::SceneManager> m_SceneManager = nullptr;

        C78E::Ref<::C78Editor::GUI::ProjectManagerUI> m_ProjectManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::AssetManagerUI> m_AssetManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::SceneManagerUI> m_SceneManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::EntityManagerUI> m_EntityManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::ViewportUI> m_ViewportUI = nullptr;

        //RendererManager?
        C78E::Ref<C78E::Renderer> m_Renderer = nullptr;

    };

}