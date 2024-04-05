#pragma once

#include "C78E/Renderer/Camera.h"

namespace C78E {

	class SceneCamera : public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		
		ProjectionType& GetProjectionType() { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }


		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		float& GetPerspectiveVerticalFOV() { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float& GetPerspectiveNearClip() { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float& GetPerspectiveFarClip() { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }
		

		void SetOrthographic(float size, float nearClip, float farClip);
		float& GetOrthographicSize() { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float& GetOrthographicNearClip() { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float& GetOrthographicFarClip() { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }


		void SetViewportSize(uint32_t width, uint32_t height);

		
		uint32_t getWidth() { return m_Width; }
		uint32_t getHeight() { return m_Height; }

		const glm::mat4& GetProjection() { RecalculateProjection(); return m_Projection; }

	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		uint32_t m_Width = 1, m_Height = 1;
		float m_AspectRatio = 1.0f;
	};

}
