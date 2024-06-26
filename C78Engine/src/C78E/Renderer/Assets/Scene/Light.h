#pragma once
#include "C78EPCH.h"

namespace C78E::Light {

	struct AmbientLight {
		glm::vec4 color{ 1.f, 1.f, 1.f, .0f };
	};

	struct DirectLight {
		glm::vec3 direction{ 0.f, -1.f, 0.f };
		alignas(16) glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
	};

	struct PointLight {
		glm::vec3 position{ 0.f, 0.f, 0.f };
		alignas(16) glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
	};

	struct SpotLight {
		glm::vec3 position{ 0.f, 0.f, 0.f };
		alignas(16) glm::vec3 direction{ 0.f, -1.f, 0.f };
		alignas(16) glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
		float angle = 30.f / 180.f * glm::pi<float>();
		float edgeAngle = 0.f;
	};

}
