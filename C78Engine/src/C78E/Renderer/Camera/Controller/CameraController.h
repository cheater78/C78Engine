#pragma once
#include "C78E/Core/Timestep.h"

#include <C78E/Renderer/Camera/Camera.h>
#include <C78E/Renderer/Camera/EditorCamera.h>
#include <C78E/Renderer/Camera/OrthographicCamera.h>


#include <C78E/Renderer/Assets/Scene/Entity/Entity.h>

#include "C78E/Events/ApplicationEvent.h"
#include "C78E/Events/MouseEvent.h"


namespace C78E {

	class FPSCameraEntityController;
	class BlenderCameraController;

	class CameraController {
	public:
		static Ref<FPSCameraEntityController> createFPS(Entity entity);
		static Ref<BlenderCameraController> createBlender();
	protected:
		CameraController();
	public:
		virtual void onUpdate(Timestep ts) = 0;
		virtual bool onEvent(Event& e) = 0;

		virtual void onResize(float width, float height) = 0;

		virtual bool isActive() const final { return m_Active; }
		virtual bool setActive(bool active) final { return m_Active = active; }
	protected:
		bool m_Active = true;
	};

	class CameraEntityController : public CameraController {
	public:
		CameraEntityController(Entity entity);

		virtual void onUpdate(Timestep ts) = 0;
		virtual bool onEvent(Event& e) = 0;

		virtual void onResize(float width, float height) = 0;

	protected:
		virtual TransformComponent& getTransformComponent() final;
		virtual CameraComponent& getCameraComponent() final;
	protected:
		Entity m_Entity;

	};

	class FPSCameraEntityController : public CameraEntityController {
	public:
		FPSCameraEntityController(Entity entity);

		virtual void onUpdate(Timestep ts) override;
		virtual bool onEvent(Event& e) override;

		virtual void onResize(float width, float height) override;

	
	public:
		struct Keys {
			KeyCode keyForward = Key::W;
			KeyCode keyBackward = Key::S;
			KeyCode keyRight = Key::D;
			KeyCode keyLeft = Key::A;
			KeyCode keyUp = Key::Space;
			KeyCode keyDown = Key::LeftShift;
		};
		struct Speed { //TODO: speed -> Physics, Sense to other settings
			glm::vec3 speed = glm::vec3{ 1.f, 1.f, 1.f };
			glm::vec2 mouseSense = glm::vec2{ 1.f, 1.f };
		};
	public:
		Keys m_Keys = Keys();
		Speed m_Speed = Speed();
		bool m_FixedXZPlane = true;
		bool m_FixedYVertical = true;

	private:
		glm::vec2 m_MousePosBeforeActive{};
	};

	class BlenderCameraController {

	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};
}
