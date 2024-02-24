#include "EnttInspector.h"
#include "imgui/imgui.h"

uint32_t EnttInspector::s_ID = entt::null;
bool EnttInspector::s_showInspector = false; // init s_ID if true!

bool EnttInspector::s_ShowDetailedTransformComponent = true;

bool EnttInspector::s_ShowDetailedCameraComponent = true;

bool EnttInspector::s_ShowDetailedAmbientLightComponent = true;
bool EnttInspector::s_ShowDetailedDirectLightComponent = true;
bool EnttInspector::s_ShowDetailedPointLightComponent = true;
bool EnttInspector::s_ShowDetailedSpotLightComponent = true;

bool EnttInspector::s_ShowDetailedMeshComponent = true;
bool EnttInspector::s_ShowDetailedMaterialComponent = true;
bool EnttInspector::s_ShowDetailedTextureComponent = true;


static constexpr ImGuiTabBarFlags s_EnttInspectorTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_NoClip | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths;
static constexpr ImGuiTableColumnFlags s_EnttInspectorTableColFlags = 0;
static constexpr ImGuiTableColumnFlags s_EnttInspectorTableHighlightRowFlags = ImGuiTableRowFlags_Headers;

void EnttInspector::init() {

}

void EnttInspector::onImGuiRender(C78E::Ref<C78E::Scene> scene) {
    showEntityList(scene);
    C78E::Entity entity((entt::entity)s_ID, scene.get());
    if (s_showInspector)
        showInspectorList(entity);
}

void EnttInspector::showEntityList(C78E::Ref<C78E::Scene> scene) {

    ImGui::Begin("EnttityList");
    auto entts = scene->getAllEntitiesWith<C78E::TagComponent, C78E::TransformComponent>();
    for (auto entt : entts) {
        ImGui::PushID((int)entt);
        C78E::Entity entity(entt, scene.get());
        if (ImGui::SmallButton(std::string(entity.getComponent<C78E::TagComponent>() + " (" + std::to_string((uint32_t)entt) + ")").c_str())) {
            s_ID = (uint32_t)entt;
            s_showInspector = true;
        }
        ImGui::PopID();
        ImGui::PushID((int)entt);
        ImGui::SameLine();
        if (ImGui::Button((entity.getComponent<C78E::StateComponent>().enable) ? "Enabled" : "Disabled")) { entity.getComponent<C78E::StateComponent>().enable = !entity.getComponent<C78E::StateComponent>().enable; }
        ImGui::PopID();
    }
    ImGui::End();

}

void EnttInspector::showInspectorList(C78E::Entity& entity) {
    if (!ImGui::Begin("Inspector", &s_showInspector)) { ImGui::End(); return; }

    if (ImGui::BeginTable("InspectorTable", 2, s_EnttInspectorTableFlags))
    {
        ImGui::TableSetupColumn("Attribute", s_EnttInspectorTableColFlags);
        ImGui::TableSetupColumn("Value", s_EnttInspectorTableColFlags);

        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImVec4(0.05f, 0.1f, 0.13f, 0.6f)));
        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImVec4(0.05f, 0.1f, 0.13f, 1.f)));
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.15f, 0.1f, 0.13f, 1.f)));

        if(true) // ID Component
        {
            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("ID ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(s_ID).c_str());
                //--------------------------------------------
            }
        }
        
        if (true) // Tag Component
        {
            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("Name ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(entity.getComponent<C78E::TagComponent>().c_str());
                //--------------------------------------------
            }
        }

        if (true) // State Component
        {
            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("Active ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                if (ImGui::Button((entity.getComponent<C78E::StateComponent>().enable) ? "True" : "False")) {
                    entity.getComponent<C78E::StateComponent>().enable = !entity.getComponent<C78E::StateComponent>().enable;
                }
                //--------------------------------------------
            }
        }

        if (entity.hasComponent<C78E::TransformComponent>()) // Transform Component
        {
            glm::vec3& translation = entity.getComponent<C78E::TransformComponent>().Translation;
            glm::vec3& rotation = entity.getComponent<C78E::TransformComponent>().Rotation;
            glm::vec3& scale = entity.getComponent<C78E::TransformComponent>().Scale;

            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("Transform");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                if (ImGui::Button("Show Transform")) {
                    s_ShowDetailedTransformComponent = !s_ShowDetailedTransformComponent;
                }
                //--------------------------------------------
            }

            if (s_ShowDetailedTransformComponent) {
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Translation ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat3("Translation", &translation[0], 0.01f, -128.f, 128.f);
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Rotation ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat3("Rotation", &rotation[0], 0.01f, -128.f, 128.f);
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Scale ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat3("Scale", &scale[0], 0.01f, -128.f, 128.f);
                    //--------------------------------------------
                }
            }

        }

        if (entity.hasComponent<C78E::CameraComponent>()) // Camera Component
        {
            C78E::SceneCamera& camera = entity.getComponent<C78E::CameraComponent>().Camera;

            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("Camera ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                if (ImGui::Button("Show Camera") ) {
                    s_ShowDetailedCameraComponent = !s_ShowDetailedCameraComponent;
                }
                //--------------------------------------------
            }


            if (s_ShowDetailedCameraComponent) {

                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("ProjectionType ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    if (ImGui::Button(
                        (camera.GetProjectionType() == C78E::SceneCamera::ProjectionType::Perspective)
                        ? std::string("Perspective").c_str() : std::string("Orthogonal").c_str()
                    )) {
                        if (camera.GetProjectionType() == C78E::SceneCamera::ProjectionType::Perspective) {
                            camera.SetProjectionType(C78E::SceneCamera::ProjectionType::Orthographic);
                        }
                        else {
                            camera.SetProjectionType(C78E::SceneCamera::ProjectionType::Perspective);
                        }
                    }
                    //--------------------------------------------
                }

                if (camera.GetProjectionType() == C78E::SceneCamera::ProjectionType::Perspective) {
                    ImGui::TableNextRow();
                    {
                        ImGui::TableNextColumn();
                        // Identifier
                        ImGui::Text("  Field Of View (vertical) ");
                        //--------------------------------------------
                        ImGui::TableNextColumn();
                        // Value
                        float fov = 180.f * camera.GetPerspectiveVerticalFOV() / glm::pi<float>();
                        ImGui::DragFloat("Field Of View (vertical)", &fov, 1.f, 20.0f, 180.0f, "%.1f");
                        if (ImGui::IsItemDeactivatedAfterEdit()) {
                            camera.SetPerspectiveVerticalFOV(glm::pi<float>()* fov / 180.0f);
                        }
                        
                        //--------------------------------------------
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableNextColumn();
                        // Identifier
                        ImGui::Text("  Near Clip ");
                        //--------------------------------------------
                        ImGui::TableNextColumn();
                        // Value
                        float nearClip = camera.GetPerspectiveNearClip();
                        float farClip = camera.GetPerspectiveFarClip();
                        ImGui::DragFloat("Near Clip", &nearClip, 0.000001f, 0.0f, farClip, "%.6f");
                        camera.SetPerspectiveNearClip(nearClip);
                        //--------------------------------------------
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableNextColumn();
                        // Identifier
                        ImGui::Text("  Far Clip ");
                        //--------------------------------------------
                        ImGui::TableNextColumn();
                        // Value
                        float nearClip = camera.GetPerspectiveNearClip();
                        float farClip = camera.GetPerspectiveFarClip();
                        ImGui::DragFloat("Far Clip", &farClip, 1.0f, nearClip, 1.f, "%.1f");
                        camera.SetPerspectiveFarClip(farClip);
                        //--------------------------------------------
                    }
                }
                else {
                    ImGui::TableNextRow();
                    {
                        ImGui::TableNextColumn();
                        // Identifier
                        ImGui::Text("  Size ");
                        //--------------------------------------------
                        ImGui::TableNextColumn();
                        // Value
                        float size = camera.GetOrthographicSize();
                        ImGui::DragFloat("Size", &size, 1.0f, 0.0f, 1000.f, "%.1f");
                        camera.SetOrthographicSize(size);
                        //--------------------------------------------
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableNextColumn();
                        // Identifier
                        ImGui::Text("  Near Clip ");
                        //--------------------------------------------
                        ImGui::TableNextColumn();
                        // Value
                        float nearClip = camera.GetOrthographicNearClip();
                        float farClip = camera.GetOrthographicFarClip();
                        ImGui::DragFloat("Near Clip", &nearClip, 0.000001f, 0.0f, farClip, "%.6f");
                        camera.SetOrthographicNearClip(nearClip);
                        //--------------------------------------------
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableNextColumn();
                        // Identifier
                        ImGui::Text("  Far Clip ");
                        //--------------------------------------------
                        ImGui::TableNextColumn();
                        // Value
                        float nearClip = camera.GetOrthographicNearClip();
                        float farClip = camera.GetOrthographicFarClip();
                        ImGui::DragFloat("Far Clip", &farClip, 1.0f, nearClip, 1.f, "%.1f");
                        camera.SetOrthographicFarClip(farClip);
                        //--------------------------------------------
                    }
                }
                
            }

        }

        if (entity.hasComponent<C78E::AmbientLightComponent>()) // Ambient Light Component
        {
            glm::vec4& color = entity.getComponent<C78E::AmbientLightComponent>().color;

            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("AmbientLight ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                if (ImGui::Button(std::string("Show AmbientLight").c_str())) {
                    s_ShowDetailedAmbientLightComponent = !s_ShowDetailedAmbientLightComponent;
                }
                //--------------------------------------------
            }

            if (s_ShowDetailedAmbientLightComponent) {
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Color: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat4("Color", &color[0], 0.01f, 0.f, 1.f, "%.2f");
                    //--------------------------------------------
                }
            }

        }

        if (entity.hasComponent<C78E::DirectLightComponent>()) // Direct Light Component
        {
            glm::vec4& color = entity.getComponent<C78E::DirectLightComponent>().color;
            glm::vec3& direction = entity.getComponent<C78E::DirectLightComponent>().direction;

            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("DirectionalLight ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                if (ImGui::Button(std::string("Show DirectionalLight").c_str())) {
                    s_ShowDetailedDirectLightComponent = !s_ShowDetailedDirectLightComponent;
                }
                //--------------------------------------------
            }

            if (s_ShowDetailedDirectLightComponent) {
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Color: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat4("Color", &color[0], 0.01f, 0.f, 1.f, "%.2f");
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Direction: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat3("Direction", &direction[0], 0.01f, 0.f, 1.f, "%.2f");
                    //--------------------------------------------
                }
            }

        }

        if (entity.hasComponent<C78E::PointLightComponent>()) // Point Light Component
        {
            glm::vec4& color = entity.getComponent<C78E::PointLightComponent>().color;
            glm::vec3& position = entity.getComponent<C78E::PointLightComponent>().position;

            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("PointLight ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                if (ImGui::Button(std::string("Show PointLight").c_str())) {
                    s_ShowDetailedPointLightComponent = !s_ShowDetailedPointLightComponent;
                }
                //--------------------------------------------
            }

            if (s_ShowDetailedPointLightComponent) {
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Color: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat4("Color", &color[0], 0.01f, 0.f, 1.f, "%.2f");
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Position: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat3("Position", &position[0], 0.01f, -1000.f, 1000.f, "%.2f");
                    //--------------------------------------------
                }
            }

        }

        if (entity.hasComponent<C78E::SpotLightComponent>()) // Spot Light Component
        {
            glm::vec4& color = entity.getComponent<C78E::SpotLightComponent>().color;
            glm::vec3& position = entity.getComponent<C78E::SpotLightComponent>().position;
            glm::vec3& direction = entity.getComponent<C78E::SpotLightComponent>().direction;
            float& angle = entity.getComponent<C78E::SpotLightComponent>().angle;
            float& edgeAngle = entity.getComponent<C78E::SpotLightComponent>().edgeAngle;

            ImGui::TableNextRow(s_EnttInspectorTableHighlightRowFlags);
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("SpotLight ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                if (ImGui::Button(std::string("Show SpotLight").c_str())) {
                    s_ShowDetailedSpotLightComponent = !s_ShowDetailedSpotLightComponent;
                }
                //--------------------------------------------
            }

            if (s_ShowDetailedSpotLightComponent) {
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Color: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat4("Color", &color[0], 0.01f, 0.f, 1.f, "%.2f");
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Position: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat3("Position", &position[0], 0.01f, -1000.f, 1000.f, "%.2f");
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Direction: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat3("Direction", &direction[0], 0.01f, -1.f, 1.f, "%.2f");
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  Angle: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat("Angle", &angle, 0.01f, 0.f, glm::two_pi<float>(), "%.2f");
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text("  SmoothAngle: ");
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::DragFloat("SmoothAngle", &edgeAngle, 0.001f, 0.f, glm::two_pi<float>(), "%.3f");
                    //--------------------------------------------
                }
            }

        }
           
        ImGui::EndTable();
    }

    if (entity.hasComponent<C78E::ModelComponent>()) {
        auto& model = entity.getComponent<C78E::ModelComponent>();
        ImGui::Text("ModelComp");
        ImGui::Text("Name: "); ImGui::SameLine(); ImGui::Text(model.models.at(0).getName().c_str());
        ImGui::Spacing();
    }

    if (entity.hasComponent<C78E::MeshComponent>()) {
        auto& mesh = entity.getComponent<C78E::MeshComponent>();
        ImGui::Text("MeshComponent");
        ImGui::Text("Name: "); ImGui::SameLine(); ImGui::Text(mesh.mesh.getName().c_str());
        ImGui::Spacing();
    }

    if (entity.hasComponent<C78E::MaterialComponent>()) {
        ImGui::Text("MaterialComponent");
        auto& mat = entity.getComponent<C78E::MaterialComponent>();
        ImGui::Text("Shader: "); ImGui::SameLine(); ImGui::Text(mat.getShader().getName().c_str());
        ImGui::Spacing();
    }

    if (entity.hasComponent<C78E::TextureComponent>()) {
        ImGui::Text("TextureComponent");
        auto& texs = entity.getComponent<C78E::TextureComponent>();
        ImGui::Text("Textures: ");
        for (auto& tex : texs.textures) {
            ImGui::Text(tex.get().getName().c_str());
        }
        ImGui::Spacing();
    }

    if (entity.hasComponent<C78E::SkyBoxComponent>()) {
        ImGui::Text("SkyBoxComponent");
        auto& sbc = entity.getComponent<C78E::SkyBoxComponent>();
        ImGui::Text("Textures: ");
        for (auto& tex : sbc.skyboxes) {
            ImGui::Text(tex.get().getName().c_str());
        }
        ImGui::Spacing();
    }
    


    ImGui::End();
}