#include "C78ePCH.h"
#include "CameraController.h"

namespace C78E {
	Ref<FPSCameraEntityController> CameraController::createFPS(Entity entity) {
		return createRef<FPSCameraEntityController>(entity);
	}
	Ref<BlenderCameraController> CameraController::createBlender() {
		return createRef<BlenderCameraController>();
	}
	CameraController::CameraController()
	{ }

	CameraEntityController::CameraEntityController(Entity entity)
		: CameraController(), m_Entity(entity) { }

	TransformComponent& CameraEntityController::getTransformComponent() { return m_Entity.getTransform(); }
	CameraComponent& CameraEntityController::getCameraComponent() { return m_Entity.getComponent<CameraComponent>(); }

	FPSCameraEntityController::FPSCameraEntityController(Entity entity)
	: CameraEntityController(entity) {
	}

	void FPSCameraEntityController::onUpdate(C78E::Timestep dt) {
		if (!m_Active) {
			if (m_MousePosBeforeActive != glm::vec2()) {
				C78E::Application::get().getWindow().setMouseMode(Window::MouseMode::Normal);
				C78E::Input::setMousePosition(m_MousePosBeforeActive.x, m_MousePosBeforeActive.y);
				m_MousePosBeforeActive = glm::vec2();
			}
			return;
		}
		if (m_MousePosBeforeActive == glm::vec2()) {
			Window& window = C78E::Application::get().getWindow();
			window.setMouseMode(Window::MouseMode::Disabled);
			glm::vec2 windowSize = { window.getWidth(), window.getHeight() };
			m_MousePosBeforeActive = glm::floor(C78E::Input::getMousePosition());
			C78E::Input::setMousePosition(windowSize.x / 2, windowSize.y / 2);
		}
		const float dtSeconds = dt.getSeconds();
		CameraComponent& cameraComponent = getCameraComponent();
		TransformComponent& transformComponent = getTransformComponent();
		auto& pos = transformComponent.translation;
		auto& rot = transformComponent.rotation;

		const glm::mat4 transform = transformComponent.getTransform();
		const glm::mat4 viewMatrix = glm::inverse(transform);
		const glm::vec3 dirRight = -glm::normalize((m_FixedXZPlane) ? (glm::vec3(viewMatrix[0]) * glm::vec3{ 1.f, 0.f, 1.f }) : viewMatrix[0]);
		const glm::vec3 dirUp = glm::normalize((m_FixedXZPlane) ? glm::vec3{ 0.f, 1.f, 0.f } : viewMatrix[1]);
		const glm::vec3 dirForward = glm::normalize((m_FixedXZPlane) ? (glm::vec3(viewMatrix[2]) * glm::vec3{ 1.f, 0.f, 1.f }) : viewMatrix[2]);

		Window& window = C78E::Application::get().getWindow();
		glm::vec2 windowSize = { window.getWidth(), window.getHeight() };
		glm::vec2 absoluteCursorPosition = glm::floor(C78E::Input::getMousePosition());
		glm::vec2 centeredCursorPosition = absoluteCursorPosition - glm::floor(windowSize / 2.f);

		glm::vec3 rotChg = dtSeconds * glm::vec3{ m_Speed.mouseSense * glm::vec2{ centeredCursorPosition.y, centeredCursorPosition.x }, 0.f };
		
		// limit all rotations to be in [-pi,pi]
		rot = glm::mod(rot + glm::radians(rotChg) + glm::pi<float>(), glm::two_pi<float>()) - glm::pi<float>();
		if (m_FixedYVertical) {
			// Limit to look straight up or down, no upside down view
			if ((rot).x < -glm::half_pi<float>()) rot.x = -glm::half_pi<float>();
			if ((rot).x > +glm::half_pi<float>()) rot.x = +glm::half_pi<float>();
		}

		glm::vec3 posChg = { 0.f, 0.f, 0.f };
		if (C78E::Input::isKeyPressed(m_Keys.keyForward)) { posChg += dirForward * dtSeconds * m_Speed.speed.z; }
		if (C78E::Input::isKeyPressed(C78E::Key::S)) { posChg -= dirForward * dtSeconds * m_Speed.speed.z; }
		if (C78E::Input::isKeyPressed(C78E::Key::A)) { posChg += dirRight * dtSeconds * m_Speed.speed.x; }
		if (C78E::Input::isKeyPressed(C78E::Key::D)) { posChg -= dirRight * dtSeconds * m_Speed.speed.x; }
		if (C78E::Input::isKeyPressed(C78E::Key::Space)) { posChg += dirUp * dtSeconds * m_Speed.speed.y; }
		if (C78E::Input::isKeyPressed(C78E::Key::LeftControl)) { posChg -= dirUp * dtSeconds * m_Speed.speed.y; }
		pos += posChg;
		
		C78E::Input::setMousePosition(windowSize.x/2, windowSize.y/2);
	}

	bool FPSCameraEntityController::onEvent(Event& e) {
		return false;
	}

	void FPSCameraEntityController::onResize(float width, float height) {
		getCameraComponent().camera.setViewportSize( static_cast<uint32_t>(width), static_cast<uint32_t>(height));
	}













	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{ }

	void OrthographicCameraController::OnUpdate(Timestep dt) {


		if (Input::isKeyPressed(Key::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt.getSeconds();
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt.getSeconds();
		}
		else if (Input::isKeyPressed(Key::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt.getSeconds();
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt.getSeconds();
		}

		if (Input::isKeyPressed(Key::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt.getSeconds();
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt.getSeconds();
		}
		else if (Input::isKeyPressed(Key::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt.getSeconds();
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt.getSeconds();
		}

		if (m_Rotation)
		{
			if (Input::isKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * dt.getSeconds();
			if (Input::isKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * dt.getSeconds();

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.setRotation(m_CameraRotation);
		}

		m_Camera.setPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e) {


		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_CALLBACK_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(BIND_CALLBACK_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height) {
		m_AspectRatio = width / height;
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {


		m_ZoomLevel -= e.getYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {


		OnResize((float)e.getWidth(), (float)e.getHeight());
		return false;
	}

}
