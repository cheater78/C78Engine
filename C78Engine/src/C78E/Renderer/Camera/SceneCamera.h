#pragma once

#include "C78E/Renderer/Camera/Camera.h"

namespace C78E {

	class SceneCamera : public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		
		ProjectionType& getProjectionType() { return m_ProjectionType; }
		void setProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }


		void setPerspective(float verticalFOV, float nearClip, float farClip);
		float& getPerspectiveVerticalFOV() { return m_PerspectiveFOV; }
		void setPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float& getPerspectiveNearClip() { return m_PerspectiveNear; }
		void setPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float& getPerspectiveFarClip() { return m_PerspectiveFar; }
		void setPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }
		

		void setOrthographic(float size, float nearClip, float farClip);
		float& getOrthographicSize() { return m_OrthographicSize; }
		void setOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float& getOrthographicNearClip() { return m_OrthographicNear; }
		void setOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float& getOrthographicFarClip() { return m_OrthographicFar; }
		void setOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }


		void setViewportSize(uint32_t width, uint32_t height);

		
		uint32_t getWidth() { return m_Width; }
		uint32_t getHeight() { return m_Height; }

		const glm::mat4& getProjection() { RecalculateProjection(); return m_Projection; }

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

namespace std {
	_EXPORT_STD _NODISCARD inline string to_string(C78E::SceneCamera::ProjectionType type) {
		switch (type) {
		case C78E::SceneCamera::ProjectionType::Perspective: return "Perspective";
		case C78E::SceneCamera::ProjectionType::Orthographic: return "Orthographic";
		default: return "Illegal ProjectionType";
		}
	}
};