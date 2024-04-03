#pragma once
#include <C78E.h>
#include <C78Elibs.h>

#include "Tools/FileManager.h"
#include "Tools/AssetManager.h"
#include "Tools/EntityInspector.h"
#include "Tools/Viewport.h"

#include "Scene/EditorCamera.h"

#include <C78E/Project/Project.h>


namespace C78Editor {

    static const C78E::FilePath projectPath = "C:\\dev\\c-cpp\\C78Engine\\C78Project\\assets\\C78Project.pce";

    class EditorLayer : public C78E::Layer {
    public:

        EditorLayer(C78E::Window& window) : Layer("EditorLayer"), m_Window(window)
        { }

        void onAttach() {
            //Create Scene and Camera for the Editor
            m_EditorScene = C78E::createRef<C78E::Scene>("EditorScene");
            m_EditorCameraEntity = m_EditorScene->createEntity("EditorCamera;");

            // TestProject - C78Project
            


            //C78_INFO("Loading Scene...");
            //C78E::SceneSerializer serializer(m_EditorScene);
            //serializer.load("assets/scenes/EditorScene");
            //C78_INFO("Done.");

            //m_EditorCameraEntity = EditorCamera::create(m_EditorScene, "EditorCamera");


            EntityInspector::init();
            AssetManager::init();
            Viewport::init();
            FileManager::init();


            



            //Shaders
            //C78E::AssetManager::loadShader("DisplayTex", "assets/shaders/Renderer3D_TexToFB.glsl");
            //C78E::AssetManager::loadShader("SkyBox", "assets/shaders/Renderer3D_SkyBox.glsl");
            //C78E::AssetManager::loadShader("Generic", "assets/shaders/Renderer3D_Generic.glsl");
            //C78E::AssetManager::loadShader("PointLight", "assets/shaders/Renderer3D_PointLight.glsl");
            //C78E::AssetManager::loadShader("SpotLight", "assets/shaders/Renderer3D_SpotLight.glsl");
            //
            //C78E::AssetManager::loadShader("GenericModel", "assets/shaders/Renderer3D_ModelShader.glsl");
            //
            //C78E::AssetManager::loadShader("RayCompute", "assets/shaders/Renderer3D_RayTracerCompute.glsl");

            //Textures
            //C78E::AssetManager::loadTexture2D("MCGrass", "assets/textures/Top.png");
            //C78E::AssetManager::loadTexture2D("test", "assets/textures/Test000.png");
            //C78E::AssetManager::loadTexture2D("woodBox", "assets/textures/WoodenCrate.jpg");
            //C78E::AssetManager::loadTexture2D("gravel", "assets/textures/GravelFloor.jpg");
            //
            //C78E::AssetManager::loadTexture2D("TreeLeafTexture", "assets/models/Trees/Leaves0156_1_S.png");
            //C78E::AssetManager::loadTexture2D("TreeBarkTexture", "assets/models/Trees/BarkDecidious0194_7_S.jpg");
            //
            //C78E::AssetManager::loadCubeMap("NightBox", "assets/textures/SkyBox/MoonSpace.png");

            //SkyBox
            //C78E::AssetManager::addMaterial("SkyBoxMaterial", C78E::createRef<C78E::Material>(
            //    C78E::Material{
            //            C78E::Material::MaterialProperties{},
            //            0, C78E::Material::MaterialTextures{},
            //            C78E::Material::MaterialPropertiesPBRext{},
            //            C78E::Material::MaterialTexturesPBRext{},
            //            C78E::AssetManager::getShaderAsset("SkyBox")
            //    }
            //));
            //
            //C78E::Entity skyBox = m_EditorScene->createEntity("SkyBox");
            //skyBox.addComponent<C78E::SkyBoxComponent>().skyboxes = { C78E::AssetManager::getCubeMapAsset("NightBox") };
            //auto& skyBoxMaterial = skyBox.addComponent<C78E::MaterialComponent>().material = C78E::AssetManager::getMaterialAsset("SkyBoxMaterial");

            //Lights
            //auto m_AmbientLight = m_EditorScene->createEntity("AmbientLight");
            //m_AmbientLight.addComponent<C78E::AmbientLightComponent>();
            //
            //auto m_DirectLight = m_EditorScene->createEntity("DirectLight");
            //m_DirectLight.addComponent<C78E::DirectLightComponent>(glm::vec3{ 0.2f, -1.f, 0.2f }, glm::vec4{ 1.f, 1.f, 1.f, .1f });
            //
            //auto m_PointLight = m_EditorScene->createEntity("PointLight");
            //m_PointLight.addComponent<C78E::PointLightComponent>().color = { 1.f, 1.f, 1.f, 5.f };
            //m_PointLight.setTransform(glm::vec3(0.f, 5.f, 8.f));
            //
            //auto m_SpotLight = m_EditorScene->createEntity("SpotLight");
            //m_SpotLight.addComponent<C78E::SpotLightComponent>(glm::vec3(), glm::vec3{ 1.f, -0.5f, 0.8f }, glm::vec4{ 1.f, 1.f, 1.f, 30.f }, 30.f / 180.f * glm::pi<float>(), 2.f / 180.f * glm::pi<float>());
            //m_SpotLight.setTransform(glm::vec3(-4.f, 2.5f, -3.f));


            //Objects
            //C78_INFO("Loading Models....");
            //
            //auto vasemodel = C78E::Model::createAll("assets/models/Misc/smooth_vase.obj").at(0);
            //vasemodel->m_Material.getRef()->setShader("GenericModel"); // normally dependend on Mat->illum
            //vasemodel->m_Material.get().getTextures().diffuse = "gravel";
            //C78E::AssetManager::addModel("smoothVase", vasemodel, vasemodel->m_Material.getSource());
            //
            //auto entity = m_EditorScene->createEntity("smoothVase");
            //entity.setTransform(glm::vec3{ 0.f, 0.f, 0.f }, glm::vec3{ 0.5f, 0.f, 0.f } *glm::two_pi<float>());
            //entity.addComponent<C78E::ModelComponent>().models = { C78E::AssetManager::getModelAsset("smoothVase") };
            //
            //C78_INFO("Done.");

            //Init Renderer
            C78E::RenderCommand::init();
            C78E::RenderCommand::setClearColor(glm::vec4(.1f, .2f, .25f, 1.f));

            C78E::Renderer::init(m_Window.getWidth(), m_Window.getHeight());
            C78E::Renderer::setDisplayShader("DisplayTex");

            C78E::Renderer::setRenderMode(C78E::Renderer::NONE);
            C78E::Renderer::onWindowResize(1960, 1080);
        }

        void onDetach() { }

        void onUpdate(C78E::Timestep delta) override {
            m_LastFrameTime = delta;

            m_EditorScene->onViewportResize(m_Window.getWidth(), m_Window.getHeight());
            if (m_MouseCapture) {
                m_Window.setMouseMode(C78E::MouseMode::DISABLED);
                EditorCamera::update(m_EditorScene->getPrimaryCamera(), delta);
            }
            else m_Window.setMouseMode(C78E::MouseMode::NORMAL);

            C78E::Renderer::submit(m_EditorScene);
            C78E::Renderer::render();
            //C78E::Renderer::display();
            Viewport::onUpdate(C78E::Renderer::getResult());
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
                ImGui::Text("FrameTime: %.1f ms", m_LastFrameTime * 1000);

                ImGui::End();
            }

            {
                ImGui::Begin("Internal");
                ImGui::Text("Camera: ");
                ImGui::Text(("  Pos: " + std::to_string(m_EditorScene->getPrimaryCamera().getComponent<C78E::TransformComponent>().Translation, 2)).c_str());
                ImGui::Text(("  Rot: " + std::to_string(m_EditorScene->getPrimaryCamera().getComponent<C78E::TransformComponent>().Rotation, 2)).c_str());
                ImGui::Text(("  Sca: " + std::to_string(m_EditorScene->getPrimaryCamera().getComponent<C78E::TransformComponent>().Scale, 2)).c_str());
                ImGui::End();
            }

            EntityInspector::onImGuiRender(m_EditorScene);
            AssetManager::onImGuiRender();
            Viewport::onImGuiRender(m_MouseCapture);
            FileManager::onImGuiRender();
        }



    private:
        C78E::Window& m_Window;
        bool m_active = true;
        C78E::Timestep m_LastFrameTime = 0;
        bool m_MouseCapture = false;


        C78E::Ref<C78E::Scene> m_EditorScene;
        C78E::Entity m_EditorCameraEntity;

        C78E::Ref<C78E::Project> m_Project = nullptr;
        
    };

}