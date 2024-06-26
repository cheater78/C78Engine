#pragma once
#include "C78EPCH.h"
#include "Light.h"

namespace C78E { //TODO: like wtf...

	struct CameraUniform {
		glm::mat4 projMat{ 1.f };
		glm::mat4 viewMat{ 1.f };
		glm::mat4 invViewMat{ 1.f };
	};

#define MAX_DIRECT_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

	struct SceneLightUniform {
		Light::AmbientLight ambientLight{ {1.f, 1.f, 1.f, .1f} };

		Light::DirectLight directLights[MAX_DIRECT_LIGHTS]{};
		int directLightCount = 0;
		alignas(16) Light::PointLight pointLights[MAX_POINT_LIGHTS]{};
		int pointLightCount = 0;
		alignas(16) Light::SpotLight spotLights[MAX_SPOT_LIGHTS]{};
		int spotLightCount = 0;
	};

	struct EntityModelUniform {
		glm::mat4 transform;
		glm::mat4 normalMat;
	};

}
