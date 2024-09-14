#include "C78EPCH.h"
#include "SceneCamera.h"

#include "C78E/Renderer/API/RendererAPI.h"


#include <glm/gtc/matrix_transform.hpp>

namespace C78E {

	SceneCamera::SceneCamera() {
		recalculateProjection();
	}

	void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip) {
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip) {
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		recalculateProjection();
	}

	void SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
		if (width <= 0 || height <= 0) return; // Minimized
		m_Width = width; m_Height = height;
		recalculateProjection();
	}

	void SceneCamera::recalculateProjection() {
		m_AspectRatio = (float)m_Width / (float)m_Height;

		if (m_ProjectionType == ProjectionType::Perspective) {
			m_Projection = perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else {
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = orthogonal(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}

}
