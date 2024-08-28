#pragma once
#include <C78E.h>
#include <C78Elibs.h>

#include "Tools/Viewport.h"


#include <C78E/Project/Project.h>

#include "GUI/EntityManager/EntityManagerUI.h"
#include "GUI/ProjectManager/ProjectManagerUI.h"

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
            m_ProjectManager(C78E::createRef<C78E::ProjectManager>()),
            m_SceneManager(C78E::createRef<C78E::SceneManager>(m_ProjectManager)),

            m_ProjectManagerUI(C78E::createRef <::C78Editor::GUI::ProjectManagerUI>(m_ProjectManager)),
            m_EntityManagerUI(C78E::createRef<::C78Editor::GUI::EntityManagerUI>(m_SceneManager))
        { }

        void onAttach() {
        }

        void onDetach() {
            if (m_ProjectManager->hasActiveProject())
                m_ProjectManager->saveProject();
        }

        void onUpdate(C78E::Timestep dt) override {
            m_LastFrameTime = dt;
            
            if (m_ProjectManager->hasActiveProject()) {
                if (!m_Renderer) {
                    m_Renderer = std::static_pointer_cast<C78E::Renderer>(C78E::createRef<C78E::Raytracer3D>(m_ProjectManager->getActiveProject()->getEditorAssetManager()));
                }

                if (m_SceneManager->hasActiveScene()) {
                    C78E::Ref<C78E::Scene> scene = m_SceneManager->getActiveScene();

                    if (!scene->hasPrimaryCamera()) {
                        C78E::Entity camera = scene->createEntity("Camera");
                        C78E::CameraComponent& camComp = camera.addComponent<C78E::CameraComponent>();
                        camComp.camera.setPerspective(45.f*glm::two_pi<float>()/360.f, 0.0001f, 1000.f);
                        camera.setTransform({ 0.f, 0.f, 0.f }, {0.f, 0.f, 0.f});
                        scene->setPrimaryCamera(camera);
                    }

                    scene->onViewportResize(m_Window.getWidth(), m_Window.getHeight());
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

                    m_Renderer->render(m_SceneManager->getActiveScene());

                    
                }
            }
            


        }

        void onEvent(C78E::Event& e) override {
            C78E::EventDispatcher dispatcher(e);
            dispatcher.dispatch<C78E::KeyPressedEvent>(BIND_CALLBACK_FN(EditorLayer::onKeyPressed));
            dispatcher.dispatch<C78E::WindowResizeEvent>(BIND_CALLBACK_FN(EditorLayer::onWindowResize));
        }

        bool onKeyPressed(C78E::KeyPressedEvent e) {
            //TODO: HotkeyManager
            //if (m_HotkeyManager->onKeyPressedEvent(e)) return true; //Hotkeys

            if (e.getKeyCode() == C78E::Key::Escape) {
                m_MouseCapture = !m_MouseCapture;
            }

            return false;
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
            


            {
                m_ProjectManagerUI->onImGuiRender();
            }

            if(m_ProjectManager->hasActiveProject()) {
                ImGui::Begin("SceneManager");

                C78E::Ref<C78E::Project> project = m_ProjectManager->getActiveProject();
                C78E::Ref<C78E::EditorAssetManager> assetManager = project->getEditorAssetManager();
                C78E::AssetRegistry assetRegistry = assetManager->getAssetRegistry();

                if (m_SceneManager->hasActiveScene()) {
                    C78E::SceneHandle sceneHandle = m_SceneManager->getActiveSceneHandle();

                    ImGui::Text(("Current Scene: " + assetManager->getMeta(sceneHandle).name).c_str());
                    if (ImGui::Button("Save")) m_SceneManager->saveScene();
                }
                else {
                    m_SceneManager->setActiveSceneHandle(m_ProjectManager->getActiveProject()->getConfig().startScene);
                }

                static C78E::GUI::TextInput createSceneTI("Create Scene:", "UnnamedScene");
                static C78E::GUI::TextButton createSceneTB("New Scene",
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
                        ImGui::SameLine();
                        if (ImGui::Button(("MakeStartupScene -" + std::to_string(handle)).c_str())) {
                            m_ProjectManager->getActiveProject()->getConfig().startScene = handle;
                        }
                    }

                }
                ImGui::End();
            }


            m_EntityManagerUI->onImGuiRender();


            /*
            if (m_SceneManager->hasActiveScene()) {
                ImGui::Begin("EntityManager");
                C78E::Ref<C78E::Scene> scene = std::static_pointer_cast<C78E::Scene>(m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->getAsset(m_SceneManager->getActiveSceneHandle()));
                static C78E::UUID activeEnt = 0;

                if (scene->hasPrimaryCamera()) {
                    C78E::Entity entity = scene->getPrimaryCamera();
                    auto& transComp = entity.getTransform();
                    auto& camComp = entity.getComponent<C78E::CameraComponent>();

                    ImGui::Text("Camera");
                    ImGui::Text("Pos: "); ImGui::SameLine(); ImGui::Text(std::to_string(transComp.Translation).c_str());
                    ImGui::Text("Rot: "); ImGui::SameLine(); ImGui::Text(std::to_string(transComp.Rotation).c_str());
                    ImGui::Text("Sca: "); ImGui::SameLine(); ImGui::Text(std::to_string(transComp.Scale).c_str());


                    ImGui::Text("FoV: "); ImGui::SameLine(); ImGui::Text(std::to_string(camComp.camera.getPerspectiveVerticalFOV()).c_str());

                    if (ImGui::Button("reset Cam")) {
                        entity.setTransform({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f });
                    }
                }

                static C78E::GUI::TextInput createEntityTI("Create Entity:", "Unnamed Entity");
                static C78E::GUI::TextButton createEntityTB("New Entity",
                    [this, scene]() {
                        scene->createEntity(createEntityTI.getContent());
                    }
                );

                createEntityTI.show();
                C78E::GUI::SameLine();
                createEntityTB.show();

                for (auto entt : scene->getAllEntitiesWith<C78E::IDComponent>()) {
                    C78E::Entity entity{ entt, scene.get()};

                    if (ImGui::Button((entity.getTag() + " (" + std::to_string(entity.getUUID()) + ")").c_str())) {
                        activeEnt = entity.getUUID();
                    }
                    ImGui::SameLine();
                    ImGui::Checkbox(("Active (" + std::to_string(entity.getUUID()) + ")").c_str(), &entity.getComponent<C78E::StateComponent>().enable);

                    

                }

                ImGui::End();

                if (activeEnt) {
                    ImGui::Begin("EntityInspector");
                    auto entt = scene->getEntityByUUID(activeEnt);
                    C78E::Entity entity{ entt, scene.get() };

                    ImGui::Text(("Name: " + entity.getTag()).c_str());

                    

                    static C78E::GUI::TextInput addMeshTI("Add Mesh", "C:\\");
                    static C78E::GUI::TextButton addMeshTB("Add Mesh",
                        [this, scene]() {
                            auto entt = scene->getEntityByUUID(activeEnt);
                            C78E::Entity entity{ entt, scene.get() };

                            C78E::FilePath path = "C:\\dev\\c-cpp\\C78Engine\\C78Project\\assets\\models\\Misc\\smooth_vase.obj";


                            C78E::Ref<C78E::WavefrontLoader::WavefrontModel> wavefrontmodel = C78E::WavefrontLoader::loadModel(path);

                            static C78E::AssetHandle shaderHandle = 0;
                            if(!shaderHandle)
                                shaderHandle = m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->importAsset("C:\\dev\\c-cpp\\C78Engine\\C78Editor\\assets\\shaders\\Renderer3D_FlatShader.glsl");

                            std::vector<C78E::Model::ModelPart> parts;

                            for (auto& wfpart : wavefrontmodel->parts) {
                                uint64_t meshID = wfpart.first;
                                uint64_t materialID = wfpart.second;

                                C78E::Model::ModelPart part;

                                {
                                    C78E::Asset::AssetMeta meshMeta;
                                    C78E::Ref<C78E::Mesh> mesh = wavefrontmodel->meshes.at(meshID);

                                    meshMeta.type = C78E::Asset::AssetType::Mesh;
                                    meshMeta.fileSource = path;
                                    meshMeta.name = wavefrontmodel->meshNames.at(meshID);
                                    part.m_Mesh = m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->addAsset(meshMeta, mesh);
                                }

                                {
                                    C78E::Asset::AssetMeta materialMeta;
                                    C78E::Ref<C78E::Material> material = wavefrontmodel->materials.at(materialID);
                                    material->m_Shader = shaderHandle;

                                    materialMeta.type = C78E::Asset::AssetType::Material;
                                    materialMeta.fileSource = path;
                                    materialMeta.name = wavefrontmodel->materialNames.at(materialID);
                                    part.m_Material = m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->addAsset(materialMeta, material);
                                }

                                parts.push_back(part);
                            }

                            C78E::Asset::AssetMeta modelMeta;
                            modelMeta.type = C78E::Asset::AssetType::Model;
                            modelMeta.fileSource = path;
                            modelMeta.name = path.filename().string();
                            C78E::AssetHandle modelHandle = m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->addAsset(modelMeta, createRef<C78E::Model>(parts));

                            entity.addComponent<C78E::ModelComponent>(modelHandle);
                        }
                    );

                    addMeshTI.show();
                    C78E::GUI::SameLine();
                    addMeshTB.show();

                    static C78E::AssetHandle cubeID = 0;
                    if (!cubeID) {
                        C78E::Asset::AssetMeta meshMeta{
                            C78E::Asset::AssetType::Mesh,
                            "/"
                            "CubeMesh"
                        };
                        C78E::Asset::AssetMeta materialMeta{
                            C78E::Asset::AssetType::Material,
                            "/"
                            "CubeMaterial"
                        };
                        C78E::Asset::AssetMeta modelMeta{
                            C78E::Asset::AssetType::Model,
                            "/"
                            "CubeModel"
                        };

                        C78E::Geometry::Cube cube{};
                        std::vector<C78E::Primitive::Vertex> verts{std::begin(cube.vertecies), std::end(cube.vertecies)};
                        std::vector<uint32_t> indecies{ std::begin(cube.indecies), std::end(cube.indecies) };

                        C78E::Ref<C78E::Mesh> mesh = C78E::createRef<C78E::Mesh>(verts, indecies);
                        C78E::AssetHandle meshHandle = m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->addAsset(meshMeta, mesh);

                        C78E::Ref<C78E::Material> material = C78E::createRef<C78E::Material>();
                        static C78E::AssetHandle shaderHandle = 0;
                        if (!shaderHandle)
                            shaderHandle = m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->importAsset("C:\\dev\\c-cpp\\C78Engine\\C78Editor\\assets\\shaders\\Renderer3D_FlatShader.glsl");
                        material->m_Shader = shaderHandle;
                        C78E::AssetHandle materialHandle = m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->addAsset(materialMeta, material);

                        C78E::Model::ModelPart modelPart{
                            meshHandle, materialHandle
                        };

                        std::vector<C78E::Model::ModelPart> parts = { modelPart };

                        cubeID = m_ProjectManager->getManager()->getActiveProject()->getEditorAssetManager()->addAsset(modelMeta, C78E::createRef<C78E::Model>(parts));
                    }

                    if (ImGui::Button("Add Cube Model") && !entity.hasComponent<C78E::ModelComponent>()) {
                        C78_EDITOR_INFO("Added Cube Model");
                        entity.setTransform({ 0.f, 0.f, 3.f });
                        entity.addComponent<C78E::ModelComponent>(cubeID);
                    }

                    ImGui::End();
                }

            }
            */
            {
                ImGui::Begin("FrameInfo");
                ImGui::Text(("FPS: " + std::to_string((uint32_t)(1 / m_LastFrameTime.getSeconds()))).c_str());
                ImGui::Text("FrameTime: %.1f ms", m_LastFrameTime.getMilliSeconds());
                ImGui::Text("FrameTime: %.0f us", m_LastFrameTime.getMilliSeconds() * 1000);

                if (m_Renderer)
                    ImGui::Text("Fragments: %1i x %1i = %1i px", m_Renderer->getTargetTexture()->getWidth(), m_Renderer->getTargetTexture()->getHeight(), m_Renderer->getTargetTexture()->getWidth() * m_Renderer->getTargetTexture()->getHeight());
                else
                    ImGui::Text("No Renderer");


                ImGui::End();
            }

            {
                if (m_ProjectManager->hasActiveProject()) {
                    ImGui::Begin("Assets");
                    for (auto& entry : m_ProjectManager->getActiveProject()->getEditorAssetManager()->getAssetRegistry()) {
                        ImGui::Text((C78E::Asset::assetTypeToString(entry.second.type) + " " + entry.second.name + " " + entry.second.fileSource.string()).c_str());
                    }
                    ImGui::End();
                }
            }

            {
                if(m_Renderer)
                    Viewport::onUpdate(m_Renderer->getTargetTexture());
                Viewport::onImGuiRender(m_MouseCapture, m_Renderer);
            }

        }



    private:
        C78E::Window& m_Window;
        C78E::Timestep m_LastFrameTime = 0;
        bool m_MouseCapture = false;

        //C78E::Ref<C78E::AssetManager> m_EditorAssetManager; //TODO: for Editor GÙI assets -> Functional: EAM and loaf files -> Goal: RuntimeManager with AssetPack

        C78E::Ref<C78E::ProjectManager> m_ProjectManager = nullptr;
        C78E::Ref<C78E::SceneManager> m_SceneManager = nullptr;




        C78E::Ref<::C78Editor::GUI::ProjectManagerUI> m_ProjectManagerUI = nullptr;
        C78E::Ref<::C78Editor::GUI::EntityManagerUI> m_EntityManagerUI = nullptr;

        //C78E::Ref<C78E::GUI::HotkeyManager> m_HotkeyManager;


        


        C78E::Ref<C78E::Renderer> m_Renderer = nullptr;

    };

}