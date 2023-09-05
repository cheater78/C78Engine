#pragma once
#include <C78e.h>
#include "imgui/imgui.h"
#include <Glad/glad.h>

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
        //camComponent.Camera.SetOrthographic(20, -10, 10);
        camComponent.Camera.SetViewportSize(m_Window.getWidth(), m_Window.getHeight());
        auto& camTransform = camera.getComponent<C78e::TransformComponent>();
        camTransform.Translation = glm::vec3(0.f, 0.f, 5.f);
        

        C78e::RenderCommand::SetClearColor(glm::vec4(.1f, .2f, .25f, 1.f));
        C78e::Renderer::Init();

        m_Shader = C78e::Shader::Create("assets/shaders/Renderer3D_Generic.glsl");
        
        float vertecies[] = {
            -.5f, -.5f, 0.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f,  0.f, 0.f,  1.f,
            +.5f, -.5f, 0.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f,  1.f, 0.f,  1.f,
            +.5f, +.5f, 0.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f,  1.f, 1.f,  1.f,
            -.5f, +.5f, 0.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f,  0.f, 1.f,  1.f,
        };

        uint32_t indicies[] = {
            0, 1, 2,  2, 3, 0/*,
            4, 5, 6,  6, 7, 4*/
        };
        
        m_VertexArray = C78e::VertexArray::Create();
        m_VertexArray->Bind();
        
        m_VertexBuffer = C78e::VertexBuffer::Create(vertecies, sizeof(vertecies));
        m_VertexBuffer->SetLayout({
            { C78e::ShaderDataType::Float3, "a_Position"     },
            { C78e::ShaderDataType::Float4, "a_Color"        },
            { C78e::ShaderDataType::Float3, "a_Normal"       },
            { C78e::ShaderDataType::Float2, "a_TexCoord"     },
            { C78e::ShaderDataType::Float,  "a_TexIndex"     },
            { C78e::ShaderDataType::Int,    "a_EntityID"     }
            });
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        m_IndexBuffer = C78e::IndexBuffer::Create(indicies, 6);
        m_IndexBuffer->Bind();
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    void onDetach() {
        //C78e::Renderer2D::Shutdown();
        C78e::Renderer::Shutdown();
    }

    void onUpdate(C78e::Timestep delta) override {
        m_LastFrameTime = delta;
        C78e::RenderCommand::Clear();


        auto cameraEntity = m_Scene->getPrimaryCameraEntity();
        auto& cam2 = cameraEntity.getComponent<C78e::CameraComponent>().Camera;
        auto& camTrans = cameraEntity.getComponent<C78e::TransformComponent>();

        if (m_MouseCapture) {
            static const float MouseSens = 0.1f;
            static const float TurnSpeed = 0.3f;
            static const float MoveSpeed = 3.f;

            auto& rot = camTrans.Rotation;
            rot -= glm::vec3( 0.f, C78e::Input::getMouseX() * delta * MouseSens, 0.f );
            rot -= glm::vec3( C78e::Input::getMouseY() * delta * MouseSens, 0.f, 0.f );
            C78e::Input::setMousePosition(0, 0);

            if (C78e::Input::isKeyPressed(C78e::Key::E)) {
                rot -= glm::vec3(0.f, 0.f, delta * TurnSpeed);
            }
            if (C78e::Input::isKeyPressed(C78e::Key::Q)) {
                rot += glm::vec3(0.f, 0.f, delta * TurnSpeed);
            }


            auto& transform = camTrans.Translation;
            

            const glm::vec3 lookat = glm::normalize(glm::vec3{ sin(rot.y), .0f, cos(rot.y) });
            const glm::vec3 upDir{ 0.f, 1.f, 0.f };
            const glm::vec3 rightDir = glm::normalize(glm::cross(lookat, upDir));
            const glm::vec3 forwardDir = glm::normalize(glm::cross(-rightDir, upDir));
            
            if (C78e::Input::isKeyPressed(C78e::Key::W)) { transform -= forwardDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::S)) { transform += forwardDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::D)) { transform -= rightDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::A)) { transform += rightDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::Space)) { transform += upDir * (float)delta * MoveSpeed; }
            if (C78e::Input::isKeyPressed(C78e::Key::LeftControl)) { transform -= upDir * (float)delta * MoveSpeed; }


        }

        auto texture = C78e::Texture2D::Create("assets/textures/bird.png");
        texture->Bind(1);


        C78e::Renderer3D::BeginScene(cameraEntity);

        C78e::Renderer3D::submit(m_Scene);

        C78e::Renderer3D::EndScene();



        /*
        C78e::Renderer2D::BeginScene(camera);


        for (int i = -10; i < 10; i++) {
            for (int j = -10; j < 10; j++) {
                C78e::Renderer2D::DrawRect(glm::translate(glm::scale(glm::mat4(1.f), glm::vec3(1.f)), glm::vec3(i, j, 0.f)), glm::vec4(0.5, 0.3, 0.8, 1.f), 1.f);
            }
        }
        
        glm::mat4 transform = glm::translate(glm::scale(glm::mat4(1.f), glm::vec3(1.f)), glm::vec3(x, y, 0.f));
        auto tex = C78e::Texture2D::Create("C:/dev/C78Engine/C78App/assets/textures/bird.png");

        C78e::Renderer2D::DrawQuad(transform, tex);


        C78e::Renderer2D::EndScene();
        */
    }

    void onEvent(C78e::Event& e) override {
        C78e::EventDispatcher dispatcher(e);
        dispatcher.dispatch<C78e::KeyPressedEvent>(BIND_CALLBACK_FN(TestLayer::onKeyPressed));
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
  

    virtual void onImGuiRender() override {
        if (!m_active) return;

        ImGui::Begin("Info");
        ImGui::Text(("FPS: " + std::to_string((1/m_LastFrameTime))).c_str());
        
        //static float color[4] = { 0.02f, 0.08f, 0.15f, 1.f };
        //ImGui::ColorPicker4("ClearColor", color);
        //C78e::RenderCommand::SetClearColor({color[0], color[1], color[2], color[3]});

        ImGui::End();
        
    }
private:
    C78e::Window& m_Window;
    bool m_active = true;
    C78e::Timestep m_LastFrameTime = 0;
    bool m_MouseCapture = false;
    
    C78e::Ref<C78e::Scene> m_Scene;

    C78e::Ref<C78e::Shader> m_Shader;
    C78e::Ref<C78e::VertexBuffer> m_VertexBuffer;
    C78e::Ref<C78e::IndexBuffer> m_IndexBuffer;
    C78e::Ref<C78e::VertexArray> m_VertexArray;

};