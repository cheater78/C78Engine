#pragma once
#include <C78E.h>
#include <C78Elibs.h>

#include "GUI/Viewport/ViewportUI.h"
#include "GUI/EntityManager/EntityManagerUI.h"
#include "GUI/AssetManager/AssetManagerUI.h"
#include "GUI/SceneManager/SceneManagerUI.h"
#include "GUI/ProjectManager/ProjectManagerUI.h"

namespace C78Editor {

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
            m_ViewportUI(C78E::createRef<::C78Editor::GUI::ViewportUI>(m_ProjectManager, m_EntityManagerUI))
        { }

        void onAttach() { }

        void onDetach() {
            if (m_ProjectManager->hasActiveProject())
                m_ProjectManager->saveProject();
        }

        void onUpdate(C78E::Timestep dt) override {
            
            if (m_ProjectManager->hasActiveProject() && m_SceneManager->hasActiveScene()) {
                m_ViewportUI->setActiveScene(m_SceneManager->getActiveScene());
                m_ViewportUI->onUpdate(dt);
            }
            
        }

        bool onEvent(C78E::Event& e) override {
            C78E::EventDispatcher dispatcher(e);
            dispatcher.dispatch<C78E::WindowResizeEvent>(BIND_CALLBACK_FN(EditorLayer::onWindowResizeEvent));
            
            if (m_ViewportUI->onEvent(e)) return true;
        }

        bool onWindowResizeEvent(C78E::WindowResizeEvent event) {
            C78E::RenderCommand::setViewport(0, 0, event.getWidth(), event.getHeight());
            return false;
        }

        void onImGuiRender() override {
            ImGui::DockSpaceOverViewport(); // For Editor -> one big root DockingSpace

            { // Menu Bar
                ImGui::BeginMainMenuBar();

                m_ProjectManagerUI->onImGuiMainMenuBar();

                if (ImGui::BeginMenu("Window")) {
                    auto& window = C78E::Application::get().getWindow();
                    C78E::Window::WindowMode mode = window.getWindowMode();
                    if (ImGui::BeginMenu("Mode")) {
                        {
                            bool b = mode == C78E::Window::WindowMode::Windowed;
                            ImGui::MenuItem("Windowed", (b) ? "" : "F11", &b, false);
                            if (b && mode != C78E::Window::WindowMode::Windowed) {
                                window.setWindowMode(C78E::Window::WindowMode::Windowed);
                            }
                        }
                        {
                            bool b = mode == C78E::Window::WindowMode::FullScreen;
                            ImGui::MenuItem("FullScreen", (b) ? "" : "F11", &b, false);
                            if (b && mode != C78E::Window::WindowMode::FullScreen) {
                                window.setWindowMode(C78E::Window::WindowMode::FullScreen);
                            }
                        }
                        {
                            bool b = mode == C78E::Window::WindowMode::BorderlessWindow;
                            ImGui::MenuItem("BorderlessWindow", "", &b, false);
                            if (b && mode != C78E::Window::WindowMode::BorderlessWindow) {
                                window.setWindowMode(C78E::Window::WindowMode::BorderlessWindow);
                            }
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            ImGui::ShowDemoWindow();
            
            m_ProjectManagerUI->onImGuiRender();
            m_AssetManagerUI->onImGuiRender();
            m_SceneManagerUI->onImGuiRender();
            m_EntityManagerUI->onImGuiRender();
            m_ViewportUI->onImGuiRender();

        }


    private:
        C78E::Window& m_Window;
        C78E::Ref<C78E::ProjectManager> m_ProjectManager = nullptr;
        C78E::Ref<C78E::SceneManager> m_SceneManager = nullptr;

        C78E::Ref<::C78Editor::GUI::ProjectManagerUI> m_ProjectManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::AssetManagerUI> m_AssetManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::SceneManagerUI> m_SceneManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::EntityManagerUI> m_EntityManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::ViewportUI> m_ViewportUI = nullptr;

    };

}