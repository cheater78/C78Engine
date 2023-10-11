#pragma once
#include <C78E.h>
#include "imgui/imgui.h"
#include <Glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGui/EnttInspector.h"

class TestLayer : public C78E::Layer {
public:
    TestLayer(C78E::Window& window): Layer("TestLayer"), m_Window(window) {
    }

    void onAttach() {
        m_Scene = C78E::createRef<C78E::Scene>();
        C78E::Entity camera = m_Scene->createEntity("MainCamera");
        auto& camComponent = camera.addComponent<C78E::CameraComponent>();
        camComponent.Primary = true;
        camComponent.Camera.SetPerspective(glm::radians<float>(45.f), 0.01f, 10000.f);
        camComponent.Camera.SetViewportSize(m_Window.getWidth(), m_Window.getHeight());
        auto& camTransform = camera.getComponent<C78E::TransformComponent>();
        camTransform.Translation = glm::vec3(0.f, 1.f, -2.f);
        
        C78E::RenderCommand::init();
        C78E::RenderCommand::setClearColor(glm::vec4(.1f, .2f, .25f, 1.f));
        C78E::Renderer3D::init();

        //Shaders
        C78E::ShaderLibrary::get()->load("assets/shaders/Renderer3D_Generic.glsl");

        // Meshes
        C78E::ModelManager::get()->load("RedTriag", C78E::GenericShape::Triangle::getMesh(glm::vec4(1.f, 0.f, 0.f, 1.f), 0));
        C78E::ModelManager::get()->load("quad1", C78E::GenericShape::Quad::getMesh(glm::vec4(1.f, 1.f, 1.f, 1.f), 1));
        C78E::ModelManager::get()->load("cube2", C78E::GenericShape::Cube::getMesh(glm::vec4(1.f, 1.f, 1.f, 1.f), 2));
        C78E::ModelManager::get()->load("cube1", C78E::GenericShape::Cube::getMesh(glm::vec4(1.f, 1.f, 1.f, 1.f), 1));

        //Materials
        C78E::Material defaultMaterial(
            C78E::ShaderLibrary::get()->get("Renderer3D_Generic"),
            {
                1.0f,
                1.0f,
                1.0f
            }
        );

        //Lights
        m_AmbientLight = m_Scene->createEntity("AmbientLight");
        auto& ambientLight = m_AmbientLight.addComponent<C78E::AmbientLightComponent>();

        m_DirectLight = m_Scene->createEntity("DirectLight");
        auto& directLight = m_DirectLight.addComponent<C78E::DirectLightComponent>();
        directLight.direction = { 0.f, -1.f, 0.f };
        directLight.color = {1.f, 1.f, 1.f, .1f};

        m_PointLight = m_Scene->createEntity("PointLight");
        auto& pointLight =  m_PointLight.addComponent<C78E::PointLightComponent>();
        pointLight.color = { 1.f, 1.f, 1.f, 1.f };
        auto& pointLightTrans = m_PointLight.getComponent<C78E::TransformComponent>();
        pointLightTrans.Translation = glm::vec3(0.f, .5f, 0.f);

        m_SpotLight = m_Scene->createEntity("SpotLight");
        auto& spotLight = m_SpotLight.addComponent<C78E::SpotLightComponent>();
        auto& spotLightTrans = m_SpotLight.getComponent<C78E::TransformComponent>();
        spotLightTrans.Translation = glm::vec3(4.f, 1.f, -4.f);
        spotLightTrans.Scale = glm::vec3(0.1f, 0.1f, 0.1f);
        spotLight.direction = {0.f, -1.f, 0.f};
        spotLight.color = { 1.f, 0.f, 0.f, 1.f };
        spotLight.angle = 30.f / 180.f * glm::pi<float>();
        spotLight.edgeAngle = 0.f;

        
        //Objects

        C78E::Entity triag = m_Scene->createEntity("Triag");
        auto& triagMesh = triag.addComponent<C78E::MeshComponent>();
        triagMesh.mesh = C78E::ModelManager::get()->get("RedTriag");
        auto& triagMat = triag.addComponent<C78E::MaterialComponent>(defaultMaterial);
        triagMat.setShader(C78E::ShaderLibrary::get()->get("Renderer3D_Generic"));
        auto& triagTrans = triag.getComponent<C78E::TransformComponent>();
        triagTrans.Translation = glm::vec3(0.f, 1.5f, 5.f);
        triagTrans.Scale = glm::vec3(5.f, 5.f, 1.f);
        
        C78E::Entity quad = m_Scene->createEntity("Quad");
        auto& quadMesh = quad.addComponent<C78E::MeshComponent>();
        quadMesh.mesh = C78E::ModelManager::get()->get("quad1");
        auto& quadMat = quad.addComponent<C78E::MaterialComponent>(defaultMaterial);
        auto& quadTrans = quad.getComponent<C78E::TransformComponent>();
        quadTrans.Translation = glm::vec3(0.f, -.00001f, 0.f);
        quadTrans.Rotation = glm::vec3(1.f*glm::half_pi<float>(), 0.f, 0.f);
        quadTrans.Scale = glm::vec3(10.f, 10.f, 1.f);

        C78E::Entity cube = m_Scene->createEntity("Cube");
        auto& cubeMesh = cube.addComponent<C78E::MeshComponent>();
        cubeMesh.mesh = C78E::ModelManager::get()->get("cube2");
        auto& cubeMat = cube.addComponent<C78E::MaterialComponent>(defaultMaterial);
        auto& cubeTrans = cube.getComponent<C78E::TransformComponent>();
        cubeTrans.Translation = glm::vec3(-2.f, 0.51f, 0.f);
        
        C78E::Entity cube2 = m_Scene->createEntity("Cube");
        auto& cube2Mesh = cube2.addComponent<C78E::MeshComponent>();
        cube2Mesh.mesh = C78E::ModelManager::get()->get("cube1");
        auto& cube2Mat = cube2.addComponent<C78E::MaterialComponent>(defaultMaterial);
        auto& cube2Trans = cube2.getComponent<C78E::TransformComponent>();
        cube2Trans.Translation = glm::vec3(4.f, 1.01f, 2.f);
        cube2Trans.Scale = glm::vec3(2.f, 2.f, 2.f);
    }

    void onDetach() {
        C78E::Renderer3D::shutdown();
    }

    void onUpdate(C78E::Timestep delta) override {
        m_LastFrameTime = delta;
        C78E::RenderCommand::setClearColor(glm::vec4(glm::vec3(m_ClearColor) * m_ClearColor.a, 1.f));
        C78E::RenderCommand::clear();

        auto cameraEntity = m_Scene->getPrimaryCameraEntity();
        cameraEntity.getComponent<C78E::CameraComponent>().Camera.SetPerspective(glm::radians<float>(m_FOV), 0.01f, 10000.f);

        if (m_MouseCapture) {
            auto& camTrans = cameraEntity.getComponent<C78E::TransformComponent>();

            static const float MouseSens = 0.1f;
            float MoveSpeed = 2.f;
            if (!C78E::Input::isKeyPressed(C78E::Key::LeftShift)) MoveSpeed *= 3;

            auto& rot = camTrans.Rotation;
            rot += glm::vec3( 0.f, C78E::Input::getMouseX() * delta * MouseSens, 0.f );
            rot += glm::vec3( C78E::Input::getMouseY() * delta * MouseSens, 0.f, 0.f );
            C78E::Input::setMousePosition(0, 0);

            const glm::vec3 lookat = glm::normalize(glm::vec3{ sin(rot.y), .0f, cos(rot.y) });
            const glm::vec3 upDir{ 0.f, 1.f, 0.f };
            const glm::vec3 rightDir = glm::normalize(glm::cross(lookat, upDir));
            const glm::vec3 forwardDir = glm::normalize(glm::cross(-rightDir, upDir));
            auto& transform = camTrans.Translation;
            if (C78E::Input::isKeyPressed(C78E::Key::W)) { transform += forwardDir * (float)delta * MoveSpeed; }
            if (C78E::Input::isKeyPressed(C78E::Key::S)) { transform -= forwardDir * (float)delta * MoveSpeed; }
            if (C78E::Input::isKeyPressed(C78E::Key::A)) { transform += rightDir * (float)delta * MoveSpeed; }
            if (C78E::Input::isKeyPressed(C78E::Key::D)) { transform -= rightDir * (float)delta * MoveSpeed; }
            if (C78E::Input::isKeyPressed(C78E::Key::Space)) { transform += upDir * (float)delta * MoveSpeed; }
            if (C78E::Input::isKeyPressed(C78E::Key::LeftControl)) { transform -= upDir * (float)delta * MoveSpeed; }
        }
        C78E::Renderer3D::beginScene(cameraEntity);

        C78E::Renderer3D::submit(m_Scene);

        C78E::Renderer3D::endScene();
    }

    void onEvent(C78E::Event& e) override {
        C78E::EventDispatcher dispatcher(e);
        dispatcher.dispatch<C78E::KeyPressedEvent>(BIND_CALLBACK_FN(TestLayer::onKeyPressed));
        dispatcher.dispatch<C78E::WindowResizeEvent>(BIND_CALLBACK_FN(TestLayer::onWindowResize));
    }

    bool onKeyPressed(C78E::KeyPressedEvent e) {
        if (e.GetKeyCode() == C78E::Key::Escape) {
            m_MouseCapture = !m_MouseCapture;
            if (m_MouseCapture) {
                m_Window.setMouseMode(C78E::MouseMode::DISABLED);
                C78E::Input::setMousePosition(0, 0);
            }
            else
                m_Window.setMouseMode(C78E::MouseMode::NORMAL);
        }
        else return false;
        return true;
    }
  
    bool onWindowResize(C78E::WindowResizeEvent e) {
        C78E::RenderCommand::setViewport(0, 0, e.getWidth(), e.getHeight());
        m_Scene->getPrimaryCameraEntity().getComponent<C78E::CameraComponent>().Camera.SetViewportSize(e.getWidth(), e.getHeight());
        return true;
    }

    void onImGuiRender() override {
        if (!m_active) return;
        ImGui::Begin("FrameInfo");
        ImGui::Text(("FPS: " + std::to_string((uint32_t)(1/m_LastFrameTime))).c_str());
        ImGui::Text(("DrawCalls: " + std::to_string(C78E::Renderer3D::getStats().DrawCalls)).c_str());
        ImGui::Text(("Vertecies: " + std::to_string(C78E::Renderer3D::getStats().Vertecies)).c_str());
        ImGui::Text(("Indicies: " + std::to_string(C78E::Renderer3D::getStats().Indicies)).c_str());
        ImGui::End();

        ImGui::Begin("Internal");
        ImGui::Text("Camera: ");
        ImGui::Text(("  Pos: " + std::to_string(glm::round(100.f * m_Scene->getPrimaryCameraEntity().getComponent<C78E::TransformComponent>().Translation) / 100.f, 2)).c_str());
        ImGui::Text(("  Rot: " + std::to_string(glm::round(100.f * m_Scene->getPrimaryCameraEntity().getComponent<C78E::TransformComponent>().Rotation) / 100.f, 2)).c_str());
        ImGui::End();

        ImGui::Begin("Tools");
        if (ImGui::Button("Reload shader")) {
            C78E::Renderer3D::setShader("assets/shaders/Renderer3D_Generic.glsl");
        }
        ImGui::Text(" ");
        ImGui::SliderFloat("vFOV", &m_FOV, 22.5f, 135.f);
        ImGui::SliderFloat4("ClearColor", &m_ClearColor[0], 0.f, 1.f);
        ImGui::Text("Lights: ");
        ImGui::SliderFloat ("  AmbientBrightness",  &m_AmbientLight.getComponent<C78E::AmbientLightComponent>().color.a, 0.f, 1.f);
        ImGui::Text(" ");
        ImGui::SliderFloat4("  DirectLight",        &m_DirectLight.getComponent<C78E::DirectLightComponent>().color[0], 0.f, 1.f);
        ImGui::SliderFloat3("  DirectLightDirect",  &m_DirectLight.getComponent<C78E::DirectLightComponent>().direction[0], -1.f, 1.f);
        ImGui::Text(" ");
        ImGui::SliderFloat3("  PointLightPos",      &m_PointLight.getComponent<C78E::TransformComponent>().Translation[0], -5.f, 5.f);
        ImGui::SliderFloat3("  PointLightColor",    &m_PointLight.getComponent<C78E::PointLightComponent>().color[0], 0.f, 1.f);
        ImGui::SliderFloat ("  PointLightIntense",  &m_PointLight.getComponent<C78E::PointLightComponent>().color[3], 0.f, 100.f);
        ImGui::Text(" ");
        ImGui::SliderFloat3("  SpotLightPos",       &m_SpotLight.getComponent<C78E::TransformComponent>().Translation[0], -5.f, 5.f);
        ImGui::SliderFloat3("  SpotLightDirect",    &m_SpotLight.getComponent<C78E::SpotLightComponent>().direction[0], -1.f, 1.f);
        ImGui::SliderFloat ("  SpotLightAngle",     &m_SpotLight.getComponent<C78E::SpotLightComponent>().angle, 0.f, glm::pi<float>());
        ImGui::SliderFloat ("  SpotLightEdgeAngle", &m_SpotLight.getComponent<C78E::SpotLightComponent>().edgeAngle, 0.f, glm::pi<float>());
        ImGui::SliderFloat3("  SpotLightColor",     &m_SpotLight.getComponent<C78E::SpotLightComponent>().color[0], 0.f, 1.f);
        ImGui::SliderFloat ("  SpotLightIntense",   &m_SpotLight.getComponent<C78E::SpotLightComponent>().color[3], 0.f, 100.f);
        ImGui::End();

        EnttInspector::onImGuiRender(m_Scene);
    }



private:
    C78E::Window& m_Window;
    bool m_active = true;
    C78E::Timestep m_LastFrameTime = 0;
    bool m_MouseCapture = false;
    
    C78E::Ref<C78E::Scene> m_Scene;

    C78E::Entity m_AmbientLight;
    C78E::Entity m_DirectLight;
    C78E::Entity m_PointLight;
    C78E::Entity m_SpotLight;


    glm::vec4 m_ClearColor{1.f, 1.f, 1.f, .1f};
    float m_FOV = 45.f;

};