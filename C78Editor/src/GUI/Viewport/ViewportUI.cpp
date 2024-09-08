#include "ViewportUI.h"

namespace C78Editor::GUI {

    ViewportUI::ViewportUI(C78E::Ref<C78E::ProjectManager> projectManager)
        : m_ProjectManager(projectManager),
		m_EditorCamera(C78E::createRef<C78E::EditorCamera>()),
        m_EditorScene(C78E::createRef<C78E::Scene>())
		{

    }

	ViewportUI::~ViewportUI() { }

    void ViewportUI::onEvent(C78E::Event& event) {
        m_EditorCamera->onEvent(event);
    }

    void ViewportUI::onUpdate(C78E::Timestep dt) {
		if (!m_Renderer) {
			if (!m_ProjectManager->hasActiveProject())
				return;
			m_Renderer = C78E::createRef<C78E::Rasterizer3D>(m_ProjectManager->getActiveProject()->getAssetManager());
		}

		m_EditorCamera->onUpdate(dt);

		if (!m_ActiveScene) return;

        m_EditorScene = C78E::Scene::copy(m_ActiveScene);

        if(!m_ViewPortCamera.isValid())
			m_Renderer->beginScene(*m_EditorCamera.get(), m_EditorCamera->getViewMatrix());
		else {
			C78E::Entity entity = m_ActiveScene->getEntityByUUID(m_ViewPortCamera);
			m_Renderer->beginScene(entity.getComponent<C78E::CameraComponent>().camera, glm::inverse(entity.getTransform().getTransform()));
		}

		m_Renderer->submit(m_EditorScene);
		m_Renderer->endScene();
    }

    void ViewportUI::onImGuiRender() {
		ImGui::Begin("Viewport");

		if (ImGui::BeginPopup("Settings")) {


			ImGui::EndPopup();
		}

		if (m_Renderer) {
			auto target = m_Renderer->getTargetTexture();
			auto targetID = target->getRendererID();
			auto targetInfo = m_Renderer->getTargetInfo();

			ImVec2 size = ImGui::GetContentRegionAvail();
			uint32_t x = static_cast<uint32_t>(size.x);
			uint32_t y = static_cast<uint32_t>(size.y);

			if (x * m_TargetScaling != target->getWidth() || y * m_TargetScaling != target->getHeight()) {
				m_Renderer->resizeTarget(x * m_TargetScaling, y * m_TargetScaling);
				m_EditorCamera->setViewportSize(x, y);
			}

			auto regionCorner = ImGui::GetCursorPos();
			ImGui::Image(
				(void*)(intptr_t)targetID,
				ImVec2{ (float)x, (float)y },
				ImVec2{ 0.f, 1.f },
				ImVec2{ 1.f, 0.f }
			);

			ImGui::SetCursorPos(regionCorner);
			ImGui::Text(("Vertecies: " + std::to_string(targetInfo.vertecies)).c_str());
			ImGui::Text(("Indicies: " + std::to_string(targetInfo.indicies)).c_str());
			ImGui::Text(("DrawCalls: " + std::to_string(targetInfo.drawCalls)).c_str());

			if (ImGui::Button("None")) {
				m_ViewPortCamera = C78E::UUID::invalid();
			}
			m_ActiveScene->getAllEntitiesWith<C78E::CameraComponent>().each(
				[this](entt::entity enttity, C78E::CameraComponent& camComponent) {
					C78E::Entity entity{ enttity, m_ActiveScene.get() };
					if (ImGui::Button((entity.getTag() + "##" + std::to_string(entity.getUUID())).c_str())) {
						m_ViewPortCamera = entity.getUUID();
					}
				}
			);
		}
		else {
			ImGui::SetCursorPos(C78E::GUI::toImVec(.5f*C78E::GUI::fromImVec(ImGui::GetContentRegionAvail())));
			ImGui::Text("No Renderer!");
		}
		ImGui::End();

    }

	void ViewportUI::setActiveScene(C78E::Ref<C78E::Scene> scene) {
		m_ActiveScene = scene;
	}

}
