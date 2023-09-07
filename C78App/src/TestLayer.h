#pragma once
#include <C78e.h>
#include "imgui/imgui.h"
#include <Glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestLayer : public C78e::Layer {
public:
    TestLayer(C78e::Window& window): Layer("TestLayer"), m_Window(window) {
    }

    void onAttach() {
        m_Scene = C78e::createRef<C78e::Scene>();
        C78e::Entity camera = m_Scene->createEntity("MainCamera");
        auto& camComponent = camera.addComponent<C78e::CameraComponent>();
        camComponent.Primary = true;
        camComponent.Camera.SetPerspective(45.f, 0.001f, 1000.f);
        camComponent.Camera.SetViewportSize(m_Window.getWidth(), m_Window.getHeight());
        auto& camTransform = camera.getComponent<C78e::TransformComponent>();
        camTransform.Translation = glm::vec3(0.f, 0.f, 0.f);
        
        C78e::RenderCommand::SetClearColor(glm::vec4(.1f, .2f, .25f, 1.f));
        C78e::Renderer3D::Init();
        
        C78e::Entity quad = m_Scene->createEntity("Quad");
        
        std::vector<C78e::Vertex> vertecies{
            C78e::Vertex( glm::vec3{-.5f, -.5f, -.5f},  glm::vec4{1.f, 1.f, 1.f, 1.0f},  glm::vec3{1.f, 1.f, 1.f},  glm::vec2{0.f, 0.f},  2.f, static_cast<int>(quad) ),
            C78e::Vertex( glm::vec3{+.5f, -.5f, -.5f},  glm::vec4{1.f, 1.f, 1.f, 1.0f},  glm::vec3{1.f, 1.f, 1.f},  glm::vec2{1.f, 0.f},  2.f, static_cast<int>(quad) ),
            C78e::Vertex( glm::vec3{+.5f, +.5f, -.5f},  glm::vec4{1.f, 1.f, 1.f, 1.0f},  glm::vec3{1.f, 1.f, 1.f},  glm::vec2{1.f, 1.f},  2.f, static_cast<int>(quad) ),
            C78e::Vertex( glm::vec3{-.5f, +.5f, -.5f},  glm::vec4{1.f, 1.f, 1.f, 1.0f},  glm::vec3{1.f, 1.f, 1.f},  glm::vec2{0.f, 1.f},  2.f, static_cast<int>(quad) ),
            C78e::Vertex( glm::vec3{-.5f, -.5f, +.5f},  glm::vec4{1.f, 1.f, 1.f, 1.0f},  glm::vec3{1.f, 1.f, 1.f},  glm::vec2{0.f, 0.f},  1.f, static_cast<int>(quad) ),
            C78e::Vertex( glm::vec3{+.5f, -.5f, +.5f},  glm::vec4{1.f, 1.f, 1.f, 1.0f},  glm::vec3{1.f, 1.f, 1.f},  glm::vec2{1.f, 0.f},  1.f, static_cast<int>(quad) ),
            C78e::Vertex( glm::vec3{+.5f, +.5f, +.5f},  glm::vec4{1.f, 1.f, 1.f, 1.0f},  glm::vec3{1.f, 1.f, 1.f},  glm::vec2{1.f, 1.f},  1.f, static_cast<int>(quad) ),
            C78e::Vertex( glm::vec3{-.5f, +.5f, +.5f},  glm::vec4{1.f, 1.f, 1.f, 1.0f},  glm::vec3{1.f, 1.f, 1.f},  glm::vec2{0.f, 1.f},  1.f, static_cast<int>(quad) )
        };
        std::vector<uint32_t> indicies{
            0, 1, 2,  2, 3, 0,
            4, 5, 6,  6, 7, 4
        };
        quad.addComponent<C78e::ModelComponent>(vertecies, indicies);
        auto& quadTrans = quad.getComponent<C78e::TransformComponent>();
        quadTrans.Translation = glm::vec3(0.f, 0.f, -2.f);

    }

    void onDetach() {
        C78e::Renderer3D::Shutdown();
    }

    void onUpdate(C78e::Timestep delta) override {
        m_LastFrameTime = delta;
        C78e::RenderCommand::Clear();

        auto cameraEntity = m_Scene->getPrimaryCameraEntity();
        auto& camTrans = cameraEntity.getComponent<C78e::TransformComponent>();

        if (m_MouseCapture) {
            static const float MouseSens = 0.1f;
            static const float TurnSpeed = 0.3f;
            static const float MoveSpeed = 3.f;

            auto& rot = camTrans.Rotation;
            rot -= glm::vec3( 0.f, C78e::Input::getMouseX() * delta * MouseSens, 0.f );
            rot -= glm::vec3( C78e::Input::getMouseY() * delta * MouseSens, 0.f, 0.f );
            C78e::Input::setMousePosition(0, 0);

            const glm::vec3 lookat = glm::normalize(glm::vec3{ sin(rot.y), .0f, cos(rot.y) });
            const glm::vec3 upDir{ 0.f, 1.f, 0.f };
            const glm::vec3 rightDir = glm::normalize(glm::cross(lookat, upDir));
            const glm::vec3 forwardDir = glm::normalize(glm::cross(-rightDir, upDir));
            auto& transform = camTrans.Translation;
            if (C78e::Input::isKeyPressed(C78e::Key::W)) { transform -= forwardDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::S)) { transform += forwardDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::D)) { transform -= rightDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::A)) { transform += rightDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::Space)) { transform += upDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::LeftControl)) { transform -= upDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::E)) { rot -= glm::vec3(0.f, 0.f, delta * TurnSpeed); }
            if (C78e::Input::isKeyPressed(C78e::Key::Q)) { rot += glm::vec3(0.f, 0.f, delta * TurnSpeed); }
        }

        C78e::Renderer3D::BeginScene(cameraEntity);

        C78e::Renderer3D::submit(m_Scene);

        C78e::Renderer3D::EndScene();
    }

    void onEvent(C78e::Event& e) override {
        C78e::EventDispatcher dispatcher(e);
        dispatcher.dispatch<C78e::KeyPressedEvent>(BIND_CALLBACK_FN(TestLayer::onKeyPressed));
        dispatcher.dispatch<C78e::WindowResizeEvent>(BIND_CALLBACK_FN(TestLayer::onWindowResize));
    }

    bool onKeyPressed(C78e::KeyPressedEvent e) {
        if (e.GetKeyCode() == C78e::Key::Escape) {
            m_MouseCapture = !m_MouseCapture;
            if (m_MouseCapture)
                m_Window.setMouseMode(C78e::MouseMode::DISABLED);
            else
                m_Window.setMouseMode(C78e::MouseMode::NORMAL);
        }
        else return false;
        return true;
    }
  
    bool onWindowResize(C78e::WindowResizeEvent e) {
        m_Scene->getPrimaryCameraEntity().getComponent<C78e::CameraComponent>().Camera.SetViewportSize(e.getWidth(), e.getHeight());
        return true;
    }

    void onImGuiRender() override {
        if (!m_active) return;
        ImGui::Begin("Info");
        ImGui::Text(("FPS: " + std::to_string((1/m_LastFrameTime))).c_str());
        ImGui::Text(("DrawCalls: " + std::to_string(C78e::Renderer3D::GetStats().DrawCalls)).c_str());
        ImGui::Text(("Vertecies: " + std::to_string(C78e::Renderer3D::GetStats().Vertecies)).c_str());
        ImGui::Text(("Indicies: " + std::to_string(C78e::Renderer3D::GetStats().Indicies)).c_str());
        ImGui::End();
    }



private:
    C78e::Window& m_Window;
    bool m_active = true;
    C78e::Timestep m_LastFrameTime = 0;
    bool m_MouseCapture = false;
    
    C78e::Ref<C78e::Scene> m_Scene;

};