#include "EnttInspector.h"
#include "imgui/imgui.h"

uint32_t EnttInspector::s_ID = entt::null;
bool EnttInspector::s_showInspector = false;

void EnttInspector::onImGuiRender(C78E::Ref<C78E::Scene> scene) {
    showEntityList(scene);
    if (s_showInspector)
        showInspectorList(scene);
}

void EnttInspector::showEntityList(C78E::Ref<C78E::Scene> scene) {

    ImGui::Begin("EnttityList");
    auto& entts = scene->getAllEntitiesWith<C78E::TagComponent, C78E::TransformComponent>();
    for (auto entt : entts) {
        ImGui::PushID((int)entt);
        C78E::Entity entity(entt, scene.get());
        if (ImGui::SmallButton(std::string(entity.getComponent<C78E::TagComponent>() + " (" + std::to_string((uint32_t)entt) + ")").c_str())) {
            s_ID = (uint32_t)entt;
            s_showInspector = true;
        }
        ImGui::PopID();
    }
    ImGui::End();

}

void EnttInspector::showInspectorList(C78E::Ref<C78E::Scene> scene) {
    if (s_ID == entt::null) return;
    
    C78E::Entity entity((entt::entity)s_ID, scene.get());
    if (!ImGui::Begin("Inspector", &s_showInspector)) { ImGui::End(); return; }

    ImGui::Text(std::string(std::string("ID: ") + std::to_string(s_ID)).c_str());
    ImGui::Text(std::string(std::string("Name: ") + entity.getComponent<C78E::TagComponent>()).c_str());
    ImGui::Spacing();
    ImGui::Text("Transform");
    ImGui::DragFloat3("Translation", &entity.getComponent<C78E::TransformComponent>().Translation[0], 0.01f, -128.f, 128.f);
    ImGui::DragFloat3("Rotation", &entity.getComponent<C78E::TransformComponent>().Rotation[0], 0.01f, -128.f, 128.f);
    ImGui::DragFloat3("Scale", &entity.getComponent<C78E::TransformComponent>().Scale[0], 0.01f, -128.f, 128.f);
    ImGui::Spacing();

    ImGui::Spacing();
    

    
    ImGui::Text("Components");

    if (entity.hasComponent<C78E::CameraComponent>()) {
        ImGui::Text("CameraComponent");
        ImGui::Spacing();
    }

    if (entity.hasComponent<C78E::MeshComponent>()) {
        auto& mesh = entity.getComponent<C78E::MeshComponent>();
        ImGui::Text("MeshComponent", mesh.mesh.get()->getName().c_str());
        ImGui::Spacing();
    }

    if (entity.hasComponent<C78E::MaterialComponent>()) {
        ImGui::Text("MaterialComponent");
        auto& mat = entity.getComponent<C78E::MaterialComponent>();
        ImGui::Text("Shader: "); ImGui::SameLine(); ImGui::Text(mat.getShader().get()->getName().c_str());
        ImGui::Spacing();
    }

    


    ImGui::End();
}