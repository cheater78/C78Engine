#include "EntityInspector.h"
#include <C78E/Assets/Model/Model.h>
#include <C78E/Project/Project.h>
#include <C78E/Assets/Material/Material.h>
#include <C78E/Assets/Shader/Shader.h>

namespace C78Editor {

    uint32_t EntityInspector::s_ID = entt::null;
    std::unordered_map<std::string, bool> EntityInspector::s_ShowElement;


    static constexpr ImGuiTableColumnFlags s_EnttityListTableColumnFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths;

    static constexpr ImGuiTabBarFlags s_EntityInspectorTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths;
    static constexpr ImGuiTableColumnFlags s_EntityInspectorTableColFlags = 0;
    static constexpr ImGuiTableColumnFlags s_EntityInspectorTableHighlightRowFlags = ImGuiTableRowFlags_Headers;

    static constexpr uint32_t indentStep = 2;
    static constexpr char indentChar = ' ';

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
                ImGui::Checkbox(std::to_string((uint32_t)entt).c_str(), &entity.getComponent<C78E::StateComponent>().enable);
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Name
                if (ImGui::SmallButton(std::string(entity.getComponent<C78E::TagComponent>().tag + " (" + std::to_string((uint32_t)entt) + ")").c_str())) {
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

            
            insIDComponent(element, entity, 0);
            insTagComponent(element, entity, 0);
            insStateComponent(element, entity, 0);
            insTransformComponent(element, entity, 0);

            if (entity.hasComponent<C78E::CameraComponent>()) // Camera Component
                insCameraComponent(element, entity, 0);

            if (entity.hasComponent<C78E::AmbientLightComponent>()) // Ambient Light Component
                insAmbinentLightComponent(element, entity, 0);
                
            if (entity.hasComponent<C78E::DirectLightComponent>()) // Direct Light Component
                insDirectLightComponent(element, entity, 0);

            if (entity.hasComponent<C78E::PointLightComponent>()) // Point Light Component
                insPointLightComponent(element, entity, 0);

            if (entity.hasComponent<C78E::SpotLightComponent>()) // Spot Light Component
                insSpotLightComponent(element, entity, 0);

            if (entity.hasComponent<C78E::ModelComponent>()) // Model Component
                insModelComponent(element, entity, 0);

            if (entity.hasComponent<C78E::MeshComponent>()) // Mesh Component
                insMeshComponent(element, entity, 0);
            
            if (entity.hasComponent<C78E::MaterialComponent>()) // Material Component
                insMaterialComponent(element, entity, 0);
            
            if (entity.hasComponent<C78E::TextureComponent>()) // Texture Component
                insTextureComponent(element, entity, 0);

            if (entity.hasComponent<C78E::SkyBoxComponent>()) // SkyBox Component
                insSkyBoxComponent(element, entity, 0);

            ImGui::EndTable();
        }
        ImGui::End();
    }

    /*
    * EntityInspector::insData
    * inserts any primitive immutable Data
    * std::to_string(const T& data) must be defined!
    */
    template<typename T>
    inline void EntityInspector::insData(std::string element, std::string title, const T& data, uint32_t indent, ImGuiTableRowFlags rowFlags) {
        ImGui::TableNextRow(rowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr(title, indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            ImGui::Text(std::to_string(data).c_str());
            //--------------------------------------------
        }
    }

    /*
    * EntityInspector::insDataOptToggleElement
    * inserts any primitive immutable Data with the Option to toggle the visibility of "element"
    * std::to_string(const T& data) must be defined!
    */
    template<typename T>
    inline void EntityInspector::insDataOptToggleElement(std::string element, std::string title, const T& data, uint32_t indent, ImGuiTableRowFlags rowFlags) {
        ImGui::TableNextRow(rowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr(title, indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::to_string(data).c_str()))
                toggleShow(element);
            //--------------------------------------------
        }
    }

    /*
    * EntityInspector::insDataOptToggleData
    * inserts any binary Type of Data with the Option to toggle it to its current counterstate
    * data must be one of state0 or state1, which have to be the only two possible states of T
    * std::to_string(const T& data) must be defined!
    */
    template<typename T>
    void EntityInspector::insDataOptToggleData(std::string element, std::string title, T& data, const T& state0, const T& state1, uint32_t indent, ImGuiTableRowFlags rowFlags) {
        C78_EDITOR_ASSERT(data == state0 || data == state1, "EntityInspector::insDataOptToggleData: data can only be one of the two specified states!");
        ImGui::TableNextRow(rowFlags);
        {
            ImGui::TableNextColumn();
            // Identifier
            ImGui::Text(indentStr(title, indent).c_str());
            //--------------------------------------------
            ImGui::TableNextColumn();
            // Value
            if (ImGui::Button(std::to_string(data).c_str()))
                if (data == state0)
                    data = state1;
                else
                    data = state0;
            //--------------------------------------------
        }
    }



    /*
    * 
    * Components
    * 
    */

    void EntityInspector::insIDComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        insData<C78E::UUID>(element + "/UUID", "UUID", entity.getUUID(), 0, s_EntityInspectorTableHighlightRowFlags);
    }

    void EntityInspector::insTagComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        insData<std::string>(element + "/Tag", "Tag", entity.getTag(), 0, s_EntityInspectorTableHighlightRowFlags);
    }

    void EntityInspector::insStateComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        insDataOptToggleData<bool>(element + "/State", "State", entity.getComponent<C78E::StateComponent>().enable, false, true, 0, s_EntityInspectorTableHighlightRowFlags);
    }

    void EntityInspector::insTransformComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/Transform";
        glm::vec3& translation = entity.getComponent<C78E::TransformComponent>().Translation;
        glm::vec3& rotation = entity.getComponent<C78E::TransformComponent>().Rotation;
        glm::vec3& scale = entity.getComponent<C78E::TransformComponent>().Scale;

        insDataOptToggleElement(element, "Transform Component", "Show", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(element)) {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                // Identifier
                ImGui::Text(indentStr("Translation", indent+1).c_str());
                //--------------------------------------------
                ImGui::TableNextColumn();
                // Value
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3((element + "/translation").c_str(), &translation[0], 0.01f, -128.f, 128.f);
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
                ImGui::DragFloat3((element + "/rotation").c_str(), &rotation[0], 0.01f, -128.f, 128.f);
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
                ImGui::DragFloat3((element + "/scale").c_str(), &scale[0], 0.01f, -128.f, 128.f);
                ImGui::PopItemWidth();
                //--------------------------------------------
            }
        }
    }

    void EntityInspector::insCameraComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/Camera";
        C78E::SceneCamera& camera = entity.getComponent<C78E::CameraComponent>().Camera;

        insDataOptToggleElement(element, "Camera Component", "Show", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(element)) {

            insDataOptToggleData<C78E::SceneCamera::ProjectionType>(element, "Projection Type", camera.GetProjectionType(), C78E::SceneCamera::ProjectionType::Orthographic, C78E::SceneCamera::ProjectionType::Perspective, indent);

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

    void EntityInspector::insAmbinentLightComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/AmbientLight";
        glm::vec4& color = entity.getComponent<C78E::AmbientLightComponent>().color;

        insDataOptToggleElement(element, "AmbientLight Component", "Show", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(element)) {
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

    void EntityInspector::insDirectLightComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/DirectLight";
        glm::vec4& color = entity.getComponent<C78E::DirectLightComponent>().color;
        glm::vec3& direction = entity.getComponent<C78E::DirectLightComponent>().direction;

        insDataOptToggleElement(element, "DirectionalLight Component", "Show", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(element)) {
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

    void EntityInspector::insPointLightComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/PointLight";
        glm::vec4& color = entity.getComponent<C78E::PointLightComponent>().color;
        glm::vec3& position = entity.getComponent<C78E::PointLightComponent>().position;

        insDataOptToggleElement(element, "PointLight Component", "Show", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(element)) {
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

    void EntityInspector::insSpotLightComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/SpotLight";
        glm::vec4& color = entity.getComponent<C78E::SpotLightComponent>().color;
        glm::vec3& position = entity.getComponent<C78E::SpotLightComponent>().position;
        glm::vec3& direction = entity.getComponent<C78E::SpotLightComponent>().direction;
        float& angle = entity.getComponent<C78E::SpotLightComponent>().angle;
        float& edgeAngle = entity.getComponent<C78E::SpotLightComponent>().edgeAngle;

        insDataOptToggleElement(element, "SpotLight Component", "Show", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(element)) {
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

    void EntityInspector::insModelComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/Model";
        auto modelHandle = entity.getComponent<C78E::ModelComponent>().model;

        // TODO: maybe unfold completly... one more step: ModelComp -> Model -> ModelData, now ModelComp = Model in viewer
        insAsset(element, "ModelComponent", modelHandle, indent);
    }

    void EntityInspector::insMeshComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/MeshComponent";
        auto& mesh = entity.getComponent<C78E::MeshComponent>().mesh;
        insAsset(element, "MeshComponent", mesh, indent);
    }

    void EntityInspector::insMaterialComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/MaterialComponent";
        auto& material = entity.getComponent<C78E::MaterialComponent>().material;
        insAsset(element, "MaterialComponent", material, indent);
    }

    void EntityInspector::insTextureComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/TextureComponent";
        auto& textures = entity.getComponent<C78E::TextureComponent>().textures;

        insDataOptToggleElement(element, "Texture Component", "Show Textures (" + std::to_string(textures.size()) + ")", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(element)) {
            uint32_t tid = 0;
            for (C78E::AssetHandle textureHandle : textures) {
                insAsset(element, "Texture #" + std::to_string(tid), textureHandle, indent + indentStep);
                tid++;
            }
        }
    }

    void EntityInspector::insSkyBoxComponent(std::string element, C78E::Entity& entity, uint32_t indent) {
        element += "/SkyBoxComponent";
        auto& skyboxes = entity.getComponent<C78E::SkyBoxComponent>().skyboxes;

        insDataOptToggleElement(element, "SkyBox Component", "Show SkyBoxes (" + std::to_string(skyboxes.size()) + ")", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(element)) {
            uint32_t sid = 0;
            for (C78E::AssetHandle skyboxHandle : skyboxes) {
                insAsset(element, "Texture #" + std::to_string(sid), skyboxHandle, indent + indentStep);
                sid++;
            }
        }
    }




    /*
    * 
    * Assets
    * 
    */

    void EntityInspector::insAsset(std::string element, std::string title, C78E::AssetHandle assetHandle, uint32_t indent) {
        element += title;

        C78E::Asset::AssetMeta assetMeta = C78E::Project::getActive()->getEditorAssetManager()->getMeta(assetHandle);

        insDataOptToggleElement<std::string>(element, title, assetMeta.name, indent);

        if (shouldShow(element)) {
            insData<C78E::AssetHandle>(element, "UUID", assetHandle, indent + indentStep); // TODO: DropDownMenu for Asset hotSwap of the same type
            insData<std::string>(element, "Name", assetMeta.name, indent + indentStep); // TODO: DropDownMenu for Asset hotSwap of the same type
            insData<std::string>(element, "Source", assetMeta.fileSource.string(), indent + indentStep);

            switch (assetMeta.type) {
            case C78E::Asset::AssetType::None: C78_EDITOR_ERROR("EntityInspector::insAsset: Can't show AssetType::None!"); break;
            case C78E::Asset::AssetType::Model: insModel(element, assetHandle, indent + indentStep); break;
            case C78E::Asset::AssetType::Mesh: insMesh(element, assetHandle, indent + indentStep); break;
            case C78E::Asset::AssetType::Material: insMaterial(element, assetHandle, indent + indentStep); break;
            case C78E::Asset::AssetType::Shader: insShader(element, assetHandle, indent + indentStep); break;
            case C78E::Asset::AssetType::Texture2D: insTexture(element, assetHandle, indent + indentStep); break;

            default: C78_EDITOR_ERROR("EntityInspector::insAsset: Can't show Project or Scene!"); break;
            }

        }

    }

    void EntityInspector::insShader(std::string element, C78E::AssetHandle shaderHandle, uint32_t indent) {
        C78E::Ref<C78E::Shader> shader = C78E::AssetManager::getAsset<C78E::Shader>(shaderHandle);

        //TODO: think about Shaders props
    }


    void EntityInspector::insModel(std::string element, C78E::AssetHandle modelHandle, uint32_t indent) {
        C78E::Ref<C78E::Model> modelRef = C78E::AssetManager::getAsset<C78E::Model>(modelHandle);

        std::string submodelBaseElement = element + "/submodels";
        insDataOptToggleElement<std::string>(submodelBaseElement, "SubModels", "Show SubModels (" + std::to_string(modelRef->m_Parts.size()) + ")", indent, s_EntityInspectorTableHighlightRowFlags);

        if (shouldShow(submodelBaseElement)) {
            uint32_t mid = 0;
            for (C78E::Model::ModelPart modelPart : modelRef->m_Parts) {
                std::string submodelElement = submodelBaseElement + "/" + std::to_string(mid);

                C78E::AssetHandle meshHandle = modelPart.m_Mesh;
                C78E::Ref<C78E::Mesh> meshRef = C78E::AssetManager::getAsset<C78E::Mesh>(meshHandle);
                C78E::Asset::AssetMeta meshMeta = C78E::Project::getActive()->getEditorAssetManager()->getMeta(meshHandle);

                C78E::AssetHandle materialHandle = modelPart.m_Material;
                C78E::Ref<C78E::Material> materialRef = C78E::AssetManager::getAsset<C78E::Material>(materialHandle);
                C78E::Asset::AssetMeta materialMeta = C78E::Project::getActive()->getEditorAssetManager()->getMeta(materialHandle);

                insDataOptToggleElement<std::string>(submodelElement, "SubModel #" + std::to_string(mid), "Show SubModel (" + meshMeta.name + "," + materialMeta.name + ")", indent + indentStep);
                
                if (shouldShow(submodelElement)) {
                    insAsset(submodelElement, "Mesh", meshHandle, indent + 2 * indentStep);
                    insAsset(submodelElement, "Material", materialHandle, indent + 2 * indentStep);
                }
                mid++;
            }
        }
    }

    void EntityInspector::insMesh(std::string element, C78E::AssetHandle meshHandle, uint32_t indent) {
        C78E::Ref<C78E::Mesh> meshRef = C78E::AssetManager::getAsset<C78E::Mesh>(meshHandle);

        insData<size_t>(element, "Vertecies", meshRef->m_Vertecies.size(), indent);
        insData<size_t>(element, "Indecies", meshRef->m_Indicies.size(), indent);
    }

    void EntityInspector::insMaterial(std::string element, C78E::AssetHandle materialHandle, uint32_t indent) {
        C78E::Ref<C78E::Material> materialRef = C78E::AssetManager::getAsset<C78E::Material>(materialHandle);
        
        insAsset(element, "Shader", materialRef->m_Shader, indent);

        insData<uint32_t>(element, "Illumination Model", materialRef->m_IlluminationModel, indent);

        //MaterialProperties
        {
            std::string materialPropertiesElement = element + "/MaterialProperties";
            C78E::Material::MaterialProperties& props = materialRef->m_MaterialProperties;
            
            insDataOptToggleElement<std::string>(materialPropertiesElement, "MaterialProperties", "Show", indent);

            if (shouldShow(materialPropertiesElement)) {
                insData<glm::vec3>(materialPropertiesElement, "Ambient (Ka)", props.ambient, indent);
                insData<glm::vec3>(materialPropertiesElement, "Diffuse (Kd)", props.diffuse, indent);
                insData<glm::vec3>(materialPropertiesElement, "Specular (Ks)", props.specular, indent);
                insData<glm::vec3>(materialPropertiesElement, "Transmittance (Tf)", props.transmittance, indent);
                insData<glm::vec3>(materialPropertiesElement, "Emission (Ke)", props.emission, indent);
                insData<float>(materialPropertiesElement, "Shininess (Ns)", props.shininess, indent);
                insData<float>(materialPropertiesElement, "Index Of Refraction (Ni)", props.ior, indent);
                insData<float>(materialPropertiesElement, "Dissolve (d)", props.dissolve, indent);
            }
        }

        //MaterialTextures
        {
            std::string materialTexturesElement = element + "/MaterialTextures";
            C78E::Material::MaterialTextures& texs = materialRef->m_MaterialTextures;

            insDataOptToggleElement<std::string>(materialTexturesElement, "MaterialTextures", "Show", indent);

            if (shouldShow(materialTexturesElement)) {
                insAsset(materialTexturesElement, "Ambient (map_Ka)", texs.ambient, indent);
                insAsset(materialTexturesElement, "Diffuse (map_Kd)", texs.diffuse, indent);
                insAsset(materialTexturesElement, "Specular (map_Ks)", texs.specular, indent);
                insAsset(materialTexturesElement, "Specular Highlight (map_Ns)", texs.specularHighlight, indent);
                insAsset(materialTexturesElement, "Bump (map_bump)", texs.bump, indent);
                insAsset(materialTexturesElement, "Displacment (disp)", texs.displacement, indent);
                insAsset(materialTexturesElement, "Alpha (map_d)", texs.alpha, indent);
                insAsset(materialTexturesElement, "Reflection (refl)", texs.reflection, indent);
            }
        }

        //MaterialPropertiesPBRext
        {
            std::string materialPropertiesPBRextElement = element + "/MaterialPropertiesPBRext";
            C78E::Material::MaterialPropertiesPBRext& props = materialRef->m_MaterialPropertiesPBRext;

            insDataOptToggleElement<std::string>(materialPropertiesPBRextElement, "MaterialPropertiesPBRext", "Show", indent);

            if (shouldShow(materialPropertiesPBRextElement)) {
                insData<float>(materialPropertiesPBRextElement, "Roughness", props.roughness, indent);
                insData<float>(materialPropertiesPBRextElement, "Metallic", props.metallic, indent);
                insData<float>(materialPropertiesPBRextElement, "Sheen", props.sheen, indent);
                insData<float>(materialPropertiesPBRextElement, "Clearcoat_thickness", props.clearcoat_thickness, indent);
                insData<float>(materialPropertiesPBRextElement, "Clearcoat_roughness", props.clearcoat_roughness, indent);
                insData<float>(materialPropertiesPBRextElement, "Anisotropy", props.anisotropy, indent);
                insData<float>(materialPropertiesPBRextElement, "Anisotropy Rotation", props.anisotropy_rotation, indent);
            }
        }

        //MaterialTexturesPBRext
        {
            std::string materialTexturesPBRextElement = element + "/MaterialTexturesPBRext";
            C78E::Material::MaterialTexturesPBRext& texs = materialRef->m_MaterialTexturesPBRext;

            insDataOptToggleElement<std::string>(materialTexturesPBRextElement, "MaterialTexturesPBRext", "Show", indent);

            if (shouldShow(materialTexturesPBRextElement)) {
                insAsset(materialTexturesPBRextElement, "Roughness", texs.roughness, indent);
                insAsset(materialTexturesPBRextElement, "Metallic", texs.metallic, indent);
                insAsset(materialTexturesPBRextElement, "Sheen", texs.sheen, indent);
                insAsset(materialTexturesPBRextElement, "Emissive", texs.emissive, indent);
                insAsset(materialTexturesPBRextElement, "Normal", texs.normal, indent);
            }
        }
    }


    void EntityInspector::insTexture(std::string element, C78E::AssetHandle textureHandle, uint32_t indent) {
        C78E::Ref<C78E::Texture> textureRef = C78E::AssetManager::getAsset<C78E::Texture>(textureHandle);
            

    }

    

    std::string EntityInspector::indentStr(std::string str, uint32_t ind) {
        for (uint32_t i = 0; i < ind; i++)
            str = indentChar + str;
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