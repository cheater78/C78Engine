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
#include "ImGui/AssetBrowser.h"
#include "ImGui/RendererViewport.h"

class TestLayer : public C78E::Layer {
public:
    TestLayer(C78E::Window& window): Layer("TestLayer"), m_Window(window) { }

    /*
    * +AssetMmg, basic AssetViewer, Model Loader (Wavefront)
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
    * Material -> AssetFallbacks
    * Error Handling
    * 
    * APP -> EDITOR
    * 
    * 
    */



    void onAttach() {

        EnttInspector::init();
        AssetBrowser::init();
        C78Editor::RendererViewport::init();


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

        C78E::AssetManager::loadShader("GenericModel", "assets/shaders/Renderer3D_ModelShader.glsl");

        C78E::AssetManager::loadShader("RayCompute","assets/shaders/Renderer3D_RayTracerCompute.glsl");

        //Textures
        C78E::AssetManager::loadTexture2D("MCGrass", "assets/textures/Top.png");
        C78E::AssetManager::loadTexture2D("test", "assets/textures/Test000.png");
        C78E::AssetManager::loadTexture2D("woodBox", "assets/textures/WoodenCrate.jpg");
        C78E::AssetManager::loadTexture2D("gravel", "assets/textures/GravelFloor.jpg");
        




        C78E::AssetManager::loadTexture2D("TreeLeafTexture", "assets/models/Trees/Leaves0156_1_S.png");
        C78E::AssetManager::loadTexture2D("TreeBarkTexture", "assets/models/Trees/BarkDecidious0194_7_S.jpg");





        C78E::AssetManager::loadCubeMap("NightBox", "assets/textures/SkyBox/MoonSpace.png");

        //SkyBox
        C78E::Entity skyBox = m_Scene->createEntity("SkyBox");
        skyBox.addComponent<C78E::SkyBoxComponent>().skyboxes = { C78E::AssetManager::getCubeMapAsset("NightBox") };
        auto& skyBoxMaterial = skyBox.addComponent<C78E::MaterialComponent>(
            C78E::Material{
                C78E::Material::MaterialProperties{},
                0, C78E::Material::MaterialTextures{},
                C78E::Material::MaterialPropertiesPBRext{},
                C78E::Material::MaterialTexturesPBRext{},
                C78E::AssetManager::getShaderAsset("SkyBox")
            }
        );

        //Lights
        m_AmbientLight = m_Scene->createEntity("AmbientLight");
        m_AmbientLight.addComponent<C78E::AmbientLightComponent>();

        m_DirectLight = m_Scene->createEntity("DirectLight");
        m_DirectLight.addComponent<C78E::DirectLightComponent>(glm::vec3{ 0.2f, -1.f, 0.2f }, glm::vec4{ 1.f, 1.f, 1.f, .1f });

        m_PointLight = m_Scene->createEntity("PointLight");
        m_PointLight.addComponent<C78E::PointLightComponent>().color = { 1.f, 1.f, 1.f, 5.f };
        m_PointLight.setTransform(glm::vec3(0.f, 5.f, 8.f));

        m_SpotLight = m_Scene->createEntity("SpotLight");
        m_SpotLight.addComponent<C78E::SpotLightComponent>(glm::vec3(), glm::vec3{ 1.f, -0.5f, 0.8f }, glm::vec4{ 1.f, 1.f, 1.f, 30.f }, 30.f / 180.f * glm::pi<float>(), 2.f / 180.f * glm::pi<float>());
        m_SpotLight.setTransform(glm::vec3(-4.f, 2.5f, -3.f));


        //Objects
        C78_INFO("Loading Models....");

        auto vasemodel = C78E::Model::createAll("assets/models/Misc/smooth_vase.obj").at(0);
        vasemodel->m_Material.getRef()->setShader("GenericModel"); // normally dependend on Mat->illum
        vasemodel->m_Material.get().getTextures().diffuse = "gravel";
        C78E::AssetManager::addModel("smoothVase", vasemodel, vasemodel->m_Material.getSource());

        auto entity = m_Scene->createEntity("smoothVase");
        entity.setTransform(glm::vec3{ 0.f, 0.f, 0.f }, glm::vec3{0.5f, 0.f, 0.f} * glm::two_pi<float>());
        entity.addComponent<C78E::ModelComponent>().models = { C78E::AssetManager::getModelAsset("smoothVase") };
        
        C78_INFO("Done.");
        
        //Init Renderer
        C78E::RenderCommand::init();
        C78E::RenderCommand::setClearColor(glm::vec4(.1f, .2f, .25f, 1.f));

        C78E::Renderer::init(m_Window.getWidth(), m_Window.getHeight());
        C78E::Renderer::setDisplayShader("DisplayTex");

        C78E::Renderer::setRenderMode(C78E::Renderer::RASTERIZE_3D);
        C78E::Renderer::onWindowResize(1960, 1080);
    }

    void onDetach() { }

    void onUpdate(C78E::Timestep delta) override {
        m_LastFrameTime = delta;

        m_Scene->onViewportResize(m_Window.getWidth(), m_Window.getHeight());
        auto cameraEntity = m_Scene->getPrimaryCamera();
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

        C78E::Renderer::submit(m_Scene);
        C78E::Renderer::render();
        //C78E::Renderer::display();
        C78Editor::RendererViewport::onUpdate(C78E::Renderer::getResult());
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
        return true;
    }

    void onImGuiRender() override {
        if (!m_active) return;

        {
            ImGui::Begin("DisplayMode");
            switch (C78E::Renderer::getRenderMode())
            {
            case C78E::Renderer::NONE:
                if (ImGui::Button("NONE"))
                    C78E::Renderer::setRenderMode(C78E::Renderer::RASTERIZE_2D);
                break;
            case C78E::Renderer::RASTERIZE_2D:
                if (ImGui::Button("RASTERIZE_2D"))
                    C78E::Renderer::setRenderMode(C78E::Renderer::RASTERIZE_3D);
                break;
            case C78E::Renderer::RASTERIZE_3D:
                if (ImGui::Button("RASTERIZE_3D"))
                    C78E::Renderer::setRenderMode(C78E::Renderer::RAYTRACE_3D);
                break;
            case C78E::Renderer::RAYTRACE_3D:
                if (ImGui::Button("RAYTRACE_3D"))
                    C78E::Renderer::setRenderMode(C78E::Renderer::NONE);
                break;
            default:
                break;
            }
            ImGui::End();
        }

        {
            ImGui::Begin("FrameInfo");
            ImGui::Text(("FPS: " + std::to_string((uint32_t)(1 / m_LastFrameTime))).c_str());
            ImGui::Text("FrameTime: %.1f ms", m_LastFrameTime*1000);
            ImGui::End();
        }

        {
            ImGui::Begin("Internal");
            ImGui::Text("Camera: ");
            ImGui::Text(("  Pos: " + std::to_string(m_Scene->getPrimaryCamera().getComponent<C78E::TransformComponent>().Translation, 2)).c_str());
            ImGui::Text(("  Rot: " + std::to_string(m_Scene->getPrimaryCamera().getComponent<C78E::TransformComponent>().Rotation, 2)).c_str());
            ImGui::Text(("  Sca: " + std::to_string(m_Scene->getPrimaryCamera().getComponent<C78E::TransformComponent>().Scale, 2)).c_str());
            ImGui::End();
        }
        

        EnttInspector::onImGuiRender(m_Scene);
        AssetBrowser::onImGuiRender();
        C78Editor::RendererViewport::onImGuiRender();
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

};