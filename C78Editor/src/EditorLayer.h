#pragma once
#include <C78E.h>
#include <C78Elibs.h>

#include "Tools/EntityInspector.h"
#include "Tools/Viewport.h"

#include "Scene/EditorCamera.h"

#include <C78E/Project/Project.h>



#include "UI/ProjectManager.h"
#include <C78E/GUI/FileSystem/core/FileSearcher.h>

namespace C78Editor {

    static const C78E::FilePath projectPath = "C:\\dev\\c-cpp\\C78Engine\\C78Project\\assets\\C78Project.pce";

    class EditorLayer : public C78E::Layer {
    public:

        EditorLayer(C78E::Window& window) : Layer("EditorLayer"), m_Window(window)
        { }

        void onAttach() {

            ProjectManager::init();

            //Create Scene and Camera for the Editor
            m_EditorScene = C78E::createRef<C78E::Scene>();
            m_EditorCameraEntity = m_EditorScene->createEntity("EditorCamera;");

            
            EntityInspector::init();
            //Viewport::init();

        }

        void onDetach() { }

        void onUpdate(C78E::Timestep delta) override {
            m_LastFrameTime = delta;

            ProjectManager::onUpdate();



            m_EditorScene->onViewportResize(m_Window.getWidth(), m_Window.getHeight());
            if (m_MouseCapture) {
                m_Window.setMouseMode(C78E::MouseMode::DISABLED);
                EditorCamera::update(m_EditorCameraEntity, delta);
            }
            else m_Window.setMouseMode(C78E::MouseMode::NORMAL);

        }

        void onEvent(C78E::Event& e) override {
            C78E::EventDispatcher dispatcher(e);
            dispatcher.dispatch<C78E::KeyPressedEvent>(BIND_CALLBACK_FN(EditorLayer::onKeyPressed));
            dispatcher.dispatch<C78E::WindowResizeEvent>(BIND_CALLBACK_FN(EditorLayer::onWindowResize));
        }

        bool onKeyPressed(C78E::KeyPressedEvent e) {
            return false;
        }

        bool onWindowResize(C78E::WindowResizeEvent e) {
            C78E::RenderCommand::setViewport(0, 0, e.getWidth(), e.getHeight());
            return true;
        }

        void onImGuiRender() override {
            if (!m_active) return;
            ImGui::DockSpaceOverViewport(); // For Editor -> one big root DockingSpace

            if (ProjectManager::onImGuiRender()) return;


            {
                ImGui::Begin("FrameInfo");
                ImGui::Text(("FPS: " + std::to_string((uint32_t)(1 / m_LastFrameTime))).c_str());
                ImGui::Text("FrameTime: %.1f ms", m_LastFrameTime * 1000);

                ImGui::End();
            }

            EntityInspector::onImGuiRender(m_EditorScene);
            //Viewport::onImGuiRender(m_MouseCapture);
        }



    private:
        C78E::Window& m_Window;
        bool m_active = true;
        C78E::Timestep m_LastFrameTime = 0;
        bool m_MouseCapture = false;


        C78E::Ref<C78E::Scene> m_EditorScene;
        C78E::Entity m_EditorCameraEntity;
    };

}