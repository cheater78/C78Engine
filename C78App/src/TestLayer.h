#pragma once
#include <C78e.h>
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
    TestLayer(C78E::Window& window): Layer("TestLayer"), m_Window(window) { }

    /*
    * +AssetMmg
    * 
    * TD:
    * 
    * Renderers CleanUp
    * Renderers Base - Abstract Tex?!
    * 
    * FileSys,
    * 
    * Asset-Base
    * Asset-UUID - AssetIdentifier?!
    * Asset Ser/DeSer - Formats/Conventions
    * 
    * RayTracer
    * 
    * Inspector
    * 
    * APP -> EDITOR
    * 
    * 
    */



    void onAttach() {
        //Create Scene and Camera
        m_Scene = C78E::createRef<C78E::Scene>();
        C78E::Entity camera = m_Scene->createEntity("MainCamera");
        auto& camComponent = camera.addComponent<C78E::CameraComponent>();
        camComponent.Camera.SetPerspective(glm::radians<float>(45.f), 0.01f, 10000.f);
        camera.setTransform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f / 360.f * glm::two_pi<float>(), 0.f, 0.f));
        m_Scene->setPrimaryCamera(camera);

        

        //Shaders
        C78E::AssetManager::loadShader("DisplayTex","assets/shaders/Renderer3D_TexToFB.glsl");
        C78E::AssetManager::loadShader("SkyBox","assets/shaders/Renderer3D_SkyBox.glsl");
        C78E::AssetManager::loadShader("Generic","assets/shaders/Renderer3D_Generic.glsl");
        C78E::AssetManager::loadShader("PointLight","assets/shaders/Renderer3D_PointLight.glsl");
        C78E::AssetManager::loadShader("SpotLight","assets/shaders/Renderer3D_SpotLight.glsl");

        C78E::AssetManager::loadShader("RayCompute","assets/shaders/Renderer3D_RayTracerCompute.glsl");

        // Meshes
        //C78E::AssetManager::loadMesh("ModelName", "Path/To/Model.mdl");

        //Textures
        C78E::AssetManager::loadTexture2D("MCGrass", "assets/textures/Top.png");
        C78E::AssetManager::loadTexture2D("test", "assets/textures/Test000.png");
        C78E::AssetManager::loadTexture2D("woodBox", "assets/textures/WoodenCrate.jpg");
        C78E::AssetManager::loadTexture2D("gravel", "assets/textures/GravelFloor.jpg");
        
        C78E::AssetManager::loadCubeMap("NightBox", "assets/textures/SkyBox/MoonSpace.png");

        //Materials
        C78E::Material defaultMaterial(
            C78E::AssetManager::getShader("Generic"),
            {
                1.0f,
                1.0f,
                1.0f
            }
        );

        //SkyBox
        C78E::Entity skyBox = m_Scene->createEntity("SkyBox");
        skyBox.addComponent<C78E::SkyBoxComponent>().skyboxes = { C78E::AssetManager::getCubeMapAsset("NightBox") };
        auto& skyBoxMaterial = skyBox.addComponent<C78E::MaterialComponent>(C78E::Material{
            C78E::AssetManager::getShader("SkyBox"),
            {}
            });

        //Lights
        m_AmbientLight = m_Scene->createEntity("AmbientLight");
        m_AmbientLight.addComponent<C78E::AmbientLightComponent>();

        m_DirectLight = m_Scene->createEntity("DirectLight");
        m_DirectLight.addComponent<C78E::DirectLightComponent>(glm::vec3{ 0.f, -1.f, 0.f }, glm::vec4{ 1.f, 1.f, 1.f, .1f });

        m_PointLight = m_Scene->createEntity("PointLight");
        m_PointLight.addComponent<C78E::PointLightComponent>().color = { 1.f, 1.f, 1.f, 5.f };
        m_PointLight.setTransform(glm::vec3(0.f, 5.f, 8.f));

        m_SpotLight = m_Scene->createEntity("SpotLight");
        m_SpotLight.addComponent<C78E::SpotLightComponent>(glm::vec3(), glm::vec3{ 1.f, -0.5f, 0.8f }, glm::vec4{ 1.f, 1.f, 1.f, 30.f }, 30.f / 180.f * glm::pi<float>(), 2.f / 180.f * glm::pi<float>());
        m_SpotLight.setTransform(glm::vec3(-4.f, 2.5f, -3.f));


        
        //Objects
        /*
        C78E::Entity quad = m_Scene->createEntity("RayQuad");
        quad.addComponent<C78E::MeshComponent>().mesh = C78E::ModelManager::get()->get("quad");
        quad.addComponent<C78E::TextureComponent>().textures = { C78E::TextureManager::get()->get("test") };
        quad.addComponent<C78E::MaterialComponent>(defaultMaterial);

        
        C78E::Entity quad = m_Scene->createEntity("Quad");
        quad.addComponent<C78E::MeshComponent>().mesh = C78E::ModelManager::get()->get("quad");
        quad.addComponent<C78E::TextureComponent>().textures = { C78E::TextureManager::get()->get("gravel") };
        quad.addComponent<C78E::MaterialComponent>(defaultMaterial);
        quad.setTransform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f * glm::half_pi<float>(), 0.f, 0.f), glm::vec3(10.f, 10.f, 1.f));


        C78E::Entity cube = m_Scene->createEntity("Cube");
        cube.addComponent<C78E::MeshComponent>().mesh = C78E::ModelManager::get()->get("cube2");
        cube.addComponent<C78E::TextureComponent>().textures = { C78E::TextureManager::get()->get("test") };
        cube.addComponent<C78E::MaterialComponent>(defaultMaterial);
        cube.setTransform(glm::vec3(0.f, 1.f, 8.f));
        
        C78E::Entity cube2 = m_Scene->createEntity("WoodenCrate");
        cube2.addComponent<C78E::MeshComponent>().mesh = C78E::ModelManager::get()->get("cube3");
        cube2.addComponent<C78E::TextureComponent>().textures = { C78E::TextureManager::get()->get("woodBox") };
        cube2.addComponent<C78E::MaterialComponent>(defaultMaterial);
        cube2.setTransform(glm::vec3(-1.f, 0.75f, 1.f), glm::vec3(), glm::vec3(1.5f, 1.5f, 1.5f));
        
        C78E::Entity cube3 = m_Scene->createEntity("Cube");
        cube3.addComponent<C78E::MeshComponent>().mesh = C78E::ModelManager::get()->get("cube1");
        cube3.addComponent<C78E::TextureComponent>().textures = { C78E::TextureManager::get()->get("mcGrass") };
        cube3.addComponent<C78E::MaterialComponent>(defaultMaterial);
        cube3.setTransform(glm::vec3(4.f, 1.0f, 4.f), glm::vec3(), glm::vec3(2.f, 2.f, 2.f));

        C78E::Entity cube4 = m_Scene->createEntity("Cube");
        cube4.addComponent<C78E::MeshComponent>().mesh = C78E::ModelManager::get()->get("cube1");
        cube4.addComponent<C78E::TextureComponent>().textures = { C78E::TextureManager::get()->get("mcGrass") };
        cube4.addComponent<C78E::MaterialComponent>(defaultMaterial);
        cube4.setTransform(glm::vec3(2.f, 1.0f, 4.f), glm::vec3(), glm::vec3(2.f, 2.f, 2.f));

        C78E::Entity cube5 = m_Scene->createEntity("Cube");
        cube5.addComponent<C78E::MeshComponent>().mesh = C78E::ModelManager::get()->get("cube1");
        cube5.addComponent<C78E::TextureComponent>().textures = { C78E::TextureManager::get()->get("mcGrass") };
        cube5.addComponent<C78E::MaterialComponent>(defaultMaterial);
        cube5.setTransform(glm::vec3(4.f, 3.0f, 4.f), glm::vec3(), glm::vec3(2.f, 2.f, 2.f));

        C78E::Entity cube6 = m_Scene->createEntity("Cube");
        cube6.addComponent<C78E::MeshComponent>().mesh = C78E::ModelManager::get()->get("cube1");
        cube6.addComponent<C78E::TextureComponent>().textures = { C78E::TextureManager::get()->get("mcGrass") };
        cube6.addComponent<C78E::MaterialComponent>(defaultMaterial);
        cube6.setTransform(glm::vec3(4.f, 1.0f, 2.f), glm::vec3(), glm::vec3(2.f, 2.f, 2.f));
        */

        //Init Renderer
        C78E::RenderCommand::init();
        C78E::RenderCommand::setClearColor(glm::vec4(.1f, .2f, .25f, 1.f));

        C78E::Renderer3D::init();
        C78E::RayTracer3Dgpu::init();
    }

    void onDetach() {
        C78E::Renderer3D::shutdown();
    }

    void onUpdate(C78E::Timestep delta) override {
        m_LastFrameTime = delta;
        C78E::RenderCommand::setClearColor(glm::vec4(glm::vec3(m_ClearColor) * m_ClearColor.a, 1.f));

        m_Scene->onViewportResize(m_Window.getWidth(), m_Window.getHeight());
        auto cameraEntity = m_Scene->getPrimaryCamera();
        cameraEntity.getComponent<C78E::CameraComponent>().Camera.SetPerspective(glm::radians<float>(m_FOV), 0.01f, 10000.f);
        
        if (m_MouseCapture) {
            auto& camTrans = cameraEntity.getComponent<C78E::TransformComponent>();

            static const float MouseSens = 0.1f;
            float MoveSpeed = 2.f;
            if (!C78E::Input::isKeyPressed(C78E::Key::LeftShift)) MoveSpeed *= 3;

            auto& rot = camTrans.Rotation;
            rot += glm::vec3( 0.f, C78E::Input::getMouseX() * delta * MouseSens, 0.f );
            rot += glm::vec3( C78E::Input::getMouseY() * delta * MouseSens, 0.f, 0.f );
            if (rot.x < -glm::half_pi<float>()) rot.x = -glm::half_pi<float>();
            if (rot.x >  glm::half_pi<float>()) rot.x =  glm::half_pi<float>();
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

        if (!m_DisplayMode) {
            C78E::Renderer3D::beginScene("TestScene");
            C78E::Renderer3D::setSceneElements(m_SceneElements);
            C78E::Renderer3D::submit(m_Scene);
            C78E::Renderer3D::endScene();

            C78E::Renderer3D::render("TestScene", m_Window.getWidth(), m_Window.getHeight());
        }
        else {
            C78E::RayTracer3Dgpu::setViewport(m_Window.getWidth(), m_Window.getHeight());
            C78E::RayTracer3Dgpu::submit(m_Scene);
            C78E::RayTracer3Dgpu::compute();
            C78E::RayTracer3Dgpu::display();
        }
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
        m_Scene->getPrimaryCamera().getComponent<C78E::CameraComponent>().Camera.SetViewportSize(e.getWidth(), e.getHeight());
        return true;
    }

    void onImGuiRender() override {
        if (!m_active) return;

        {
            ImGui::Begin("DisplayMode");
            if (ImGui::Button( (m_DisplayMode) ? "RenderMode: RayTrace" : "RenderMode: Rasterize"))
                m_DisplayMode = !m_DisplayMode;
            ImGui::End();
        }

        {
            ImGui::Begin("FrameInfo");
            ImGui::Text(("FPS: " + std::to_string((uint32_t)(1 / m_LastFrameTime))).c_str());
            ImGui::Text("FrameTime: %.3f", m_LastFrameTime*1000);
            //ImGui::Text(("DrawCalls: " + std::to_string(C78E::Renderer3D::getStats("TestScene").drawCalls)).c_str());
            //ImGui::Text(("Vertecies: " + std::to_string(C78E::Renderer3D::getStats("TestScene").vertecies)).c_str());
            //ImGui::Text(("Indicies: " + std::to_string(C78E::Renderer3D::getStats("TestScene").indicies)).c_str());
            ImGui::End();
        }

        {
            ImGui::Begin("Internal");
            ImGui::Text("Camera: ");
            ImGui::Text(("  Pos: " + std::to_string(glm::round(100.f * m_Scene->getPrimaryCamera().getComponent<C78E::TransformComponent>().Translation) / 100.f, 2)).c_str());
            ImGui::Text(("  Rot: " + std::to_string(glm::round(100.f * m_Scene->getPrimaryCamera().getComponent<C78E::TransformComponent>().Rotation) / 100.f, 2)).c_str());
            ImGui::End();
        }
        
        {
            ImGui::Begin("Tools");
            uint32_t id = 0;
            for (auto shaderName : C78E::AssetManager::getAllShader()) {
                ImGui::PushID(id++);
                if (ImGui::Button(shaderName.c_str())) {
                    C78E::AssetManager::loadShader(shaderName);
                }
                ImGui::PopID();
            }
            ImGui::Text(" ");
            ImGui::SliderFloat("vFOV", &m_FOV, 22.5f, 135.f);
            ImGui::SliderFloat4("ClearColor", &m_ClearColor[0], 0.f, 1.f);

            if (ImGui::Button("SkyBox")) {
                m_SceneElements.skyBox = !m_SceneElements.skyBox;
            }
            if (ImGui::Button("TexMesh")) {
                ImGui::SameLine();
                m_SceneElements.texMesh = !m_SceneElements.texMesh;
            }
            if (ImGui::Button("PointLightSprites")) {
                ImGui::SameLine();
                m_SceneElements.pointLightSprites = !m_SceneElements.pointLightSprites;
            }
            if (ImGui::Button("SpotLightSprites")) {
                ImGui::SameLine();
                m_SceneElements.spotLightSprites = !m_SceneElements.spotLightSprites;
            }

            ImGui::End();
        }
        

        EnttInspector::onImGuiRender(m_Scene);
    }



private:
    C78E::Window& m_Window;
    bool m_active = true;
    C78E::Timestep m_LastFrameTime = 0;
    bool m_MouseCapture = false;
    
    C78E::Renderer3D::Renderer3DSceneElements m_SceneElements{};
    C78E::Ref<C78E::Scene> m_Scene;

    C78E::Entity m_AmbientLight;
    C78E::Entity m_DirectLight;
    C78E::Entity m_PointLight;
    C78E::Entity m_SpotLight;


    glm::vec4 m_ClearColor{1.f, 1.f, 1.f, .1f};
    float m_FOV = 45.f;


    bool m_DisplayMode = false;

};