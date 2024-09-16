#include "ViewportUI.h"

namespace C78Editor::GUI {

    ViewportUI::ViewportUI(C78E::Ref<C78E::ProjectManager> projectManager, C78E::Ref<EntityManagerUI> entityManagerUI)
        : m_ProjectManager(projectManager), m_EntityManagerUI(entityManagerUI),
		m_EditorCamera(C78E::createRef<C78E::EditorCamera>()),
        m_EditorScene(C78E::createRef<C78E::Scene>())
		{

    }

	ViewportUI::~ViewportUI() { }

    bool ViewportUI::onEvent(C78E::Event& event) {
		if (m_EditorCamera->onEvent(event)) return true;
		if (camController)
			if (camController->onEvent(event)) return true;
		return event.Handled;
    }

    void ViewportUI::onUpdate(C78E::Timestep dt) {
		m_LastFrameTime = dt;
		if (!m_Renderer) {
			if (!m_ProjectManager->hasActiveProject())
				return;
			m_Renderer = C78E::createRef<C78E::Rasterizer3D>(m_ProjectManager->getActiveProject()->getAssetManager());
		}

		m_EditorCamera->onUpdate(dt);

		if (!m_ActiveScene) return;

        m_EditorScene = C78E::Scene::copy(m_ActiveScene);

        if(!m_ViewPortCamera.isValid() || C78E::Input::isKeyPressed(C78E::Key::Escape)) {
			m_ViewPortCamera = C78E::AssetHandle::invalid();
			m_Renderer->beginScene(*m_EditorCamera.get(), m_EditorCamera->getViewMatrix());
			if (camController)
				camController->setActive(false);
		}
		else {
			C78E::Entity entity = m_ActiveScene->getEntityByUUID(m_ViewPortCamera);
			m_Renderer->beginScene(entity.getComponent<C78E::CameraComponent>().camera, glm::inverse(entity.getTransform().getTransform()));
			if (!camController) {
				camController = C78E::CameraController::createFPS(entity);
			}
			camController->setActive(true);
		}
		if (camController)
			camController->onUpdate(dt);

		m_Renderer->submit(m_EditorScene);
		m_Renderer->endScene();
    }

	bool rayHitsSphere(glm::vec3 rayOrig, glm::vec3 rayDirection, glm::vec3 sphereOrigin, float sphereRadius, glm::vec2& hits) {
		glm::vec3 L = sphereOrigin - rayOrig;
		float tca = glm::dot(L, rayDirection);
		if (tca < 0) return false;
		float d2 = glm::dot(L, L) - tca * tca;
		if (d2 > sphereRadius * sphereRadius) return false;
		float thc = glm::sqrt(sphereRadius * sphereRadius - d2);
		hits.x = tca - thc;
		hits.y = tca + thc;
		return true;
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

			const ImVec2 contentRegionSize = ImGui::GetContentRegionAvail();
			const ImVec2 regionCorner = ImGui::GetCursorPos();

			if (contentRegionSize.x * m_TargetScaling != target->getWidth() || contentRegionSize.y * m_TargetScaling != target->getHeight()) {
				m_Renderer->resizeTarget(contentRegionSize.x * m_TargetScaling, contentRegionSize.y * m_TargetScaling);
				m_EditorCamera->setViewportSize(contentRegionSize.x, contentRegionSize.y);
				if (m_ViewPortCamera.isValid())
					m_ActiveScene->getEntityByUUID(m_ViewPortCamera).getComponent<C78E::CameraComponent>().camera.setViewportSize(contentRegionSize.x, contentRegionSize.y);
			}

			ImGui::Image( (void*)(intptr_t)targetID, contentRegionSize, ImVec2{ 0.f, 1.f }, ImVec2{ 1.f, 0.f } );

			if (ImGui::IsItemClicked() && !m_ViewPortCamera.isValid() && !C78E::Input::isKeyPressed(C78E::Key::LeftAlt)) {
				
				const glm::vec3 position = m_EditorCamera->getPosition() - (m_EditorCamera->getDistance() * m_EditorCamera->getForwardDirection());
				const glm::vec2 viewPortSize = glm::vec2{ m_Renderer->getTargetTexture()->getWidth(), m_Renderer->getTargetTexture()->getHeight() };
				float fov = m_EditorCamera->getFOV();
				float degreePerPixel = fov / viewPortSize.y;

				const glm::vec2 centeredMouseCoordinates = C78E::GUI::fromImVec(ImGui::GetMousePos())
					- C78E::GUI::fromImVec(ImGui::GetWindowPos()) // remove current Window Position
					- C78E::GUI::fromImVec(ImGui::GetWindowContentRegionMin()) // remove ContentRegionOffset relative to the current Window Position
					- 0.5f * C78E::GUI::fromImVec(contentRegionSize); // center relative to ContentRegion

				const glm::vec3 forwardDir = m_EditorCamera->getForwardDirection();
				const glm::vec3 direction = glm::vec3(
					glm::rotate(glm::mat4(1.f), glm::radians(centeredMouseCoordinates.y * degreePerPixel), glm::vec3(1.f,0.f,0.f)) *
					glm::rotate(glm::mat4(1.f), glm::radians(centeredMouseCoordinates.x * degreePerPixel), glm::vec3(0.f, 1.f, 0.f)) *
					glm::vec4(forwardDir, 0.f));
				
				float shortestDistance = std::numeric_limits<float>::infinity();
				C78E::Entity closestEntity;

				for (auto& enttity : m_ActiveScene->getAllEntitiesWith<C78E::TransformComponent>()) {
					C78E::Entity entity{ enttity, m_ActiveScene.get() };

					const glm::vec3 center = entity.getTransform().translation;

					const float cosAng = glm::dot(glm::normalize(center - position), direction);
					if (cosAng < 0) continue;
					

					glm::vec2 hits;
					if (!rayHitsSphere(position, direction, center, m_SelectEntitySize, hits)) continue;

					const float normalDiskDist = ((hits.x + hits.y) / 2.f);
					const glm::vec3 normalDiskHit = normalDiskDist * direction + position;
					float currentDistance = glm::length(normalDiskHit - center);
					if (hits.y > 0 && currentDistance < shortestDistance) {
						shortestDistance = currentDistance;
						closestEntity = entity;
					}

				}
				if (closestEntity) {
					m_EntityManagerUI->selectEntity(closestEntity.getUUID());
				}
			}

			ImGui::SetCursorPos(regionCorner);
			ImGui::Text(("FPS: " + std::to_string((uint32_t)(1 / m_LastFrameTime.getSeconds()))).c_str());
			ImGui::Text("FrameTime: %.1f ms", m_LastFrameTime.getMilliSeconds());
			ImGui::Text("FrameTime: %.0f us", m_LastFrameTime.getMilliSeconds() * 1000);

			ImGui::Text(("Vertecies: " + std::to_string(targetInfo.vertecies)).c_str());
			ImGui::Text(("Indicies: " + std::to_string(targetInfo.indicies)).c_str());
			ImGui::Text(("DrawCalls: " + std::to_string(targetInfo.drawCalls)).c_str());

			if (m_Renderer)
				ImGui::Text("Fragments: %1i x %1i = %1i px", m_Renderer->getTargetTexture()->getWidth(), m_Renderer->getTargetTexture()->getHeight(), m_Renderer->getTargetTexture()->getWidth() * m_Renderer->getTargetTexture()->getHeight());
			else
				ImGui::Text("No Renderer");

			ImGui::Spacing();
			ImGui::Text("Camera:");
			if (ImGui::Button("None")) {
				m_ViewPortCamera = C78E::UUID::invalid();
			}
			m_ActiveScene->getAllEntitiesWith<C78E::CameraComponent>().each(
				[this, contentRegionSize](entt::entity enttity, C78E::CameraComponent& camComponent) {
					C78E::Entity entity{ enttity, m_ActiveScene.get() };
					if (ImGui::Button((entity.getTag() + "##" + std::to_string(entity.getUUID())).c_str())) {
						m_ViewPortCamera = entity.getUUID();
						entity.getComponent<C78E::CameraComponent>().camera.setViewportSize(contentRegionSize.x, contentRegionSize.y);
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
