#include "EntityInspector.h"

namespace C78Editor {

    uint32_t EntityInspector::s_ID = entt::null;
    std::unordered_map<std::string, bool> EntityInspector::s_ShowElement;


    static constexpr ImGuiTableColumnFlags s_EnttityListTableColumnFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths;

    static constexpr ImGuiTabBarFlags s_EntityInspectorTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths;
    static constexpr ImGuiTableColumnFlags s_EntityInspectorTableColFlags = 0;
    static constexpr ImGuiTableColumnFlags s_EntityInspectorTableHighlightRowFlags = ImGuiTableRowFlags_Headers;

    void EntityInspector::init() {
        s_ID = 0;
    }

    void EntityInspector::onImGuiRender(C78E::Ref<C78E::Scene> scene) {
        showEntityList("", scene);
        C78E::Entity entity((entt::entity)s_ID, scene.get());
        showInspectorList("",entity);
    }

    
    void EntityInspector::showEntityList(std::string element, C78E::Ref<C78E::Scene> scene) {
        element += "/EnttityList";
        if(!ImGui::Begin("EnttityList")) return;
        auto entts = scene->getAllEntitiesWith<C78E::TagComponent, C78E::TransformComponent>();
        
        if (!ImGui::BeginTable("Entities", 2, s_EnttityListTableColumnFlags)) { ImGui::End(); return; };
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Load
            ImGui::Text("Enable");
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Name
            ImGui::Text("Name");
            //--------------------------------------------
        }
        for (auto entt : entts) {
            C78E::Entity entity(entt, scene.get());
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Loaded
                if (ImGui::Checkbox(std::to_string((uint32_t)entt).c_str(), &entity.getComponent<C78E::StateComponent>().enable));
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Name
                if (ImGui::SmallButton(std::string(entity.getComponent<C78E::TagComponent>() + " (" + std::to_string((uint32_t)entt) + ")").c_str())) {
                    s_ID = (uint32_t)entt;
                    shouldShow("/Inspector") = true;
                }
                //--------------------------------------------
            }
        }
        ImGui::EndTable();

        ImGui::End();

    }

    void EntityInspector::showInspectorList(std::string element, C78E::Entity& entity) {
        element += "/Inspector";

        if (!ImGui::Begin("Inspector", &shouldShow(element))) { ImGui::End(); return; }

        if (ImGui::BeginTable("InspectorTable", 2, s_EntityInspectorTableFlags))
        {
            ImGui::TableSetupColumn("Attribute", s_EntityInspectorTableColFlags);
            ImGui::TableSetupColumn("Value", s_EntityInspectorTableColFlags);

            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImVec4(0.05f, 0.1f, 0.13f, 0.6f)));
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImVec4(0.05f, 0.1f, 0.13f, 1.f)));
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.15f, 0.1f, 0.13f, 1.f)));

            insID(element + "/ID", entity, 0);
            insTag(element + "/Tag", entity, 0);
            insState(element + "/State", entity, 0);
            insTransform(element + "/Transform", entity, 0);

            if (entity.hasComponent<C78E::CameraComponent>()) // Camera Component
                insCamera(element, entity, 0);

            if (entity.hasComponent<C78E::AmbientLightComponent>()) // Ambient Light Component
                insAmbinentLight(element, entity, 0);
                
            if (entity.hasComponent<C78E::DirectLightComponent>()) // Direct Light Component
                insDirectLight(element, entity, 0);

            if (entity.hasComponent<C78E::PointLightComponent>()) // Point Light Component
                insPointLight(element, entity, 0);

            if (entity.hasComponent<C78E::SpotLightComponent>()) // Spot Light Component
                insSpotLight(element, entity, 0);

            if (entity.hasComponent<C78E::ModelComponent>()) // Model Component
                insModel(element, entity, 0);

            if (entity.hasComponent<C78E::MeshComponent>()) // Mesh Component
                insMesh(element, entity, 0);
            
            if (entity.hasComponent<C78E::MaterialComponent>()) // Material Component
                insMaterial(element, entity, 0);
            
            if (entity.hasComponent<C78E::TextureComponent>()) // Texture Component
                insTexture(element, entity, 0);

            if (entity.hasComponent<C78E::SkyBoxComponent>()) // SkyBox Component
                insSkyBox(element, entity, 0);

            ImGui::EndTable();
        }
        ImGui::End();
    }

    void EntityInspector::insID(std::string element, C78E::Entity& entity, uint32_t indent) {
        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("ID ", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(std::to_string(entity.getComponent<C78E::IDComponent>()).c_str());
            //--------------------------------------------
        }
    }

    void EntityInspector::insTag(std::string element, C78E::Entity& entity, uint32_t indent) {
        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Name ", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(entity.getComponent<C78E::TagComponent>().c_str());
            //--------------------------------------------
        }
    }

    void EntityInspector::insState(std::string element, C78E::Entity& entity, uint32_t indent) {
        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Active ", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button((entity.getComponent<C78E::StateComponent>().enable) ? "True" : "False")) {
                entity.getComponent<C78E::StateComponent>().enable = !entity.getComponent<C78E::StateComponent>().enable;
            }
            //--------------------------------------------
        }
    }

    void EntityInspector::insTransform(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/TransformComponent";
        glm::vec3& translation = entity.getComponent<C78E::TransformComponent>().Translation;
        glm::vec3& rotation = entity.getComponent<C78E::TransformComponent>().Rotation;
        glm::vec3& scale = entity.getComponent<C78E::TransformComponent>().Scale;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Transform", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button("Show Transform"))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Translation", indent+1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3((element + "/details" + "/translation").c_str(), &translation[0], 0.01f, -128.f, 128.f);
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Rotation", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3((element + "/details" + "/rotation").c_str(), &rotation[0], 0.01f, -128.f, 128.f);
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Scale", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3((element + "/details" + "/scale").c_str(), &scale[0], 0.01f, -128.f, 128.f);
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insCamera(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/CameraComponent";
        C78E::SceneCamera& camera = entity.getComponent<C78E::CameraComponent>().Camera;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Camera", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button("Show Camera"))
                toggleShow(element + "/details");
            //--------------------------------------------
        }


        if (shouldShow(element + "/details")) {

            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("ProjectionType", indent+1).c_str());
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

            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("perspective vFOV", indent+1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                float fov = 180.f * camera.GetPerspectiveVerticalFOV() / glm::pi<float>();
                ImGui::DragFloat((element + "/details" + "/pfov").c_str(), &camera.GetPerspectiveVerticalFOV(), .01f, glm::pi<float>() * 0.1f, glm::pi<float>() * 0.95f, "%.01f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("perspective Near Plane", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat((element + "/details" + "/pNear").c_str(), &camera.GetPerspectiveNearClip(), 0.000001f, 0.0f, camera.GetPerspectiveFarClip(), "%.6f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("perspective Far Plane", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat((element + "/details" + "/pFar").c_str(), &camera.GetPerspectiveFarClip(), 1.0f, camera.GetPerspectiveNearClip(), 1000.f, "%.1f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }

            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("orthographic Size", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat((element + "/details" + "/oSize").c_str(), &camera.GetOrthographicSize(), 1.0f, 0.0f, 1000.f, "%.1f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("orthographic Near Plane", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat((element + "/details" + "/oNear").c_str(), &camera.GetOrthographicNearClip(), 0.000001f, 0.0f, camera.GetOrthographicFarClip(), "%.6f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("orthographic Far Plane", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat((element + "/details" + "/oFar").c_str(), &camera.GetOrthographicFarClip(), 1.0f, camera.GetOrthographicNearClip(), 1000.f, "%.1f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }

        }
    }

    void EntityInspector::insAmbinentLight(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/AmbientLightComponent";
        glm::vec4& color = entity.getComponent<C78E::AmbientLightComponent>().color;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("AmbientLight ", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::string("Show AmbientLight").c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Color", indent+1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat4((element + "/details" + "/color").c_str(), &color[0], 0.01f, 0.f, 1.f, "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insDirectLight(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/DirectLightComponent";
        glm::vec4& color = entity.getComponent<C78E::DirectLightComponent>().color;
        glm::vec3& direction = entity.getComponent<C78E::DirectLightComponent>().direction;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("DirectionalLight", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::string("Show DirectionalLight").c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Color", indent+1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat4((element + "/details" + "/color").c_str(), &color[0], 0.01f, 0.f, 1.f, "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Direction", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3((element + "/details" + "/direction").c_str(), &direction[0], 0.01f, 0.f, 1.f, "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insPointLight(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/PointLightComponent";
        glm::vec4& color = entity.getComponent<C78E::PointLightComponent>().color;
        glm::vec3& position = entity.getComponent<C78E::PointLightComponent>().position;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("PointLight", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::string("Show PointLight").c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Color", indent+1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat4((element + "/details" + "/color").c_str(), &color[0], 0.01f, 0.f, 1.f, "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Position", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3((element + "/details" + "/position").c_str(), &position[0], 0.01f, -1000.f, 1000.f, "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insSpotLight(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/SpotLightComponent";
        glm::vec4& color = entity.getComponent<C78E::SpotLightComponent>().color;
        glm::vec3& position = entity.getComponent<C78E::SpotLightComponent>().position;
        glm::vec3& direction = entity.getComponent<C78E::SpotLightComponent>().direction;
        float& angle = entity.getComponent<C78E::SpotLightComponent>().angle;
        float& edgeAngle = entity.getComponent<C78E::SpotLightComponent>().edgeAngle;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("SpotLight", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::string("Show SpotLight").c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Color", indent+1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat4((element + "/details" + "/color").c_str(), &color[0], 0.01f, 0.f, 1.f, "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Position", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3((element + "/details" + "/position").c_str(), &position[0], 0.01f, -1000.f, 1000.f, "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Direction", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3((element + "/details" + "/direction").c_str(), &direction[0], 0.01f, -1.f, 1.f, "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Angle", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat((element + "/details" + "/angle").c_str(), &angle, 0.01f, 0.f, glm::two_pi<float>(), "%.2f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("EdgeAngle", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat((element + "/details" + "/edgeAngle").c_str(), &edgeAngle, 0.001f, 0.f, glm::two_pi<float>(), "%.3f");
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insModel(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/ModelComponent";
        auto& models = entity.getComponent<C78E::ModelComponent>().models;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Model", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::string("Show Models").c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Models", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(models.size()).c_str());
                //--------------------------------------------
            }

            uint32_t mid = 0;
            for (auto& model : models) { // Per Model Asset
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text(indentStr("#" + std::to_string(mid++), indent + 1).c_str());
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::Text(model.getName().c_str());
                    //--------------------------------------------
                }
                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text(indentStr("Source", indent + 2).c_str());
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::Text(model.getSource().c_str());
                    //--------------------------------------------
                }

                insMesh(element, model.getRef()->m_Mesh, indent + 2);
                insMaterial(element, model.getRef()->m_Material, indent + 2);
            }
        }
    }

    void EntityInspector::insMesh(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/MeshComponent";
        auto& mesh = entity.getComponent<C78E::MeshComponent>().mesh;
        insMesh(element, mesh, indent);
    }

    void EntityInspector::insMaterial(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/MaterialComponent";
        auto& material = entity.getComponent<C78E::MaterialComponent>().material;
        insMaterial(element, material, indent);
    }

    void EntityInspector::insShader(std::string element, C78E::Asset<C78E::Shader> shader, uint32_t indent) {
        element += "/MaterialComponent";
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Name", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(shader.getName().c_str());
            //--------------------------------------------
        }
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Source", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(shader.getSource().c_str());
            //--------------------------------------------
        }
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Texture", indent + 1).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(shader.getName().c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Name", indent).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(shader.getRef()->getName().c_str());
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insTexture(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/TextureComponent";
        auto& textures = entity.getComponent<C78E::TextureComponent>().textures;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Texture", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::string("Show Textures").c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("Textures ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(textures.size()).c_str());
                //--------------------------------------------
            }
            for (auto& texture : textures) {
                insTexture(element, texture, indent + 1);
            }
        }
    }

    void EntityInspector::insSkyBox(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/SkyBoxComponent";
        auto& skyboxes = entity.getComponent<C78E::SkyBoxComponent>().skyboxes;

        ImGui::TableNextRow(s_EntityInspectorTableHighlightRowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("SkyBoxes", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::string("Show Textures").c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text("CubeMaps ");
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(skyboxes.size()).c_str());
                //--------------------------------------------
            }
            for (auto& texture : skyboxes) {
                insCubeMap(element, texture, indent + 1);
            }
        }
    }

    void EntityInspector::insModel(std::string element, C78E::Asset<C78E::Model> model, uint32_t indent) {
        element += "/Model";
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Name", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(model.getName().c_str());
            //--------------------------------------------
        }
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Source", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(model.getSource().c_str());
            //--------------------------------------------
        }

        insMesh(element, model.getRef()->m_Mesh, indent + 1);
        insMaterial(element, model.getRef()->m_Material, indent + 1);
    }

    void EntityInspector::insMesh(std::string element, C78E::Asset<C78E::Mesh> mesh, uint32_t indent) {
        element += "/Mesh";

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Mesh", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(mesh.getName().c_str())) // TODO: HotSwap
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Name", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(mesh.getName().c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Source", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(mesh.getSource().c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Vertecies", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(mesh.getRef()->m_Vertecies.size()).c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Indecies", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(mesh.getRef()->m_Indicies.size()).c_str());
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insMaterial(std::string element, C78E::Asset<C78E::Material> material, uint32_t indent) {
        element += "/Material";

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Material", indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(material.getName().c_str())) // TODO: HotSwap
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Name", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(material.getName().c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Source", indent + 1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(material.getSource().c_str());
                //--------------------------------------------
            }

            insShader(element, material.getRef()->getShader(), indent + 1);

            // Material Textures
            {
                auto& matTextures = material.getRef()->getTextures();

                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    // Identifier
                    ImGui::Text(indentStr("MaterialTextures", indent + 1).c_str());
                    //--------------------------------------------
                    ImGui::TableNextColumn();
                    // Value
                    ImGui::Text("");
                    //--------------------------------------------
                }

                insMaterialTexture(element, "ambient", matTextures.ambient, matTextures.ambientOpt, indent + 2);
                insMaterialTexture(element, "diffuse", matTextures.diffuse, matTextures.diffuseOpt, indent + 2);
                insMaterialTexture(element, "specular", matTextures.specular, matTextures.specularOpt, indent + 2);
                insMaterialTexture(element, "specularHighlight", matTextures.specularHighlight, matTextures.specularHighlightOpt, indent + 2);
                insMaterialTexture(element, "bump", matTextures.bump, matTextures.bumpOpt, indent + 2);
                insMaterialTexture(element, "displacement", matTextures.displacement, matTextures.displacementOpt, indent + 2);
                insMaterialTexture(element, "alpha", matTextures.alpha, matTextures.alphaOpt, indent + 2);
                insMaterialTexture(element, "reflection", matTextures.reflection, matTextures.reflectionOpt, indent + 2);
            }
        }
    }

    void EntityInspector::insTexture(std::string element, C78E::Asset<C78E::Texture2D> texture, uint32_t indent) {
        element += "/Texture";
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Name", indent + 1).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(texture.getName().c_str());
            //--------------------------------------------
        }
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Source", indent + 1).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(texture.getSource().c_str());
            //--------------------------------------------
        }
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Texture", indent + 1).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(texture.getName().c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Name", indent + 2).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(texture.getRef()->getName().c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Width", indent + 2).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(texture.getRef()->getWidth()).c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Height", indent + 2).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(texture.getRef()->getHeight()).c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Data", indent + 2).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImVec2 space = ImGui::GetContentRegionAvail();
                ImGui::Image((ImTextureID)texture.getRef()->getRendererID(), space);
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insCubeMap(std::string element, C78E::Asset<C78E::CubeMap> texture, uint32_t indent) {
        element += "/CubeMap";
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Name", indent + 1).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(texture.getName().c_str());
            //--------------------------------------------
        }
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Source", indent + 1).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(texture.getSource().c_str());
            //--------------------------------------------
        }
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr("Texture", indent + 1).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(texture.getName().c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }

        if (shouldShow(element + "/details")) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Name", indent + 2).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(texture.getRef()->getName().c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Size", indent + 2).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::Text(std::to_string(texture.getRef()->getSize()).c_str());
                //--------------------------------------------
            }
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Data", indent + 2).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImVec2 space = ImGui::GetContentRegionAvail();
                ImGui::Image((ImTextureID)texture.getRef()->getRendererID(), space);
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insMaterialTexture(std::string element, std::string name, std::string& texture, C78E::Texture::TextureOption option, uint32_t indent) {
        element += "/MaterialTexture" + name;
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr(name, indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (texture == "") {
                ImGui::Text("unset");
                return;
            }
            if (ImGui::Button(("Show " + name).c_str()))
                toggleShow(element + "/details");
            //--------------------------------------------
        }
        if (shouldShow(element + "/details")) {
            C78E::Asset<C78E::Texture2D> texAsset = C78E::AssetManager::getTexture2DAsset(texture);
            insTexture(element, texAsset, indent + 1);
        }
    }

    std::string EntityInspector::indentStr(std::string str, uint32_t ind) {
        static const std::string indentationSpacer = "   ";
        for (uint32_t i = 0; i < ind; i++)
            str = indentationSpacer + str;
        return str;
    }

    bool& EntityInspector::shouldShow(std::string element) {
        if (!s_ShowElement.contains(element))
            s_ShowElement.emplace(element, false);
        return s_ShowElement.at(element);
    }

    void EntityInspector::toggleShow(std::string element) {
        bool& showElement = shouldShow(element);
        showElement = !showElement;
    }
}