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

	struct MaterialUniform {
		glm::vec3 ambient = { 1.f, 1.f, 1.f };			// Ka: Ambient Color Reaction [0,1]³
		alignas(16) glm::vec3 diffuse = { 1.f, 1.f, 1.f };			// Kd: Diffuse Color Reaction [0,1]³
		alignas(16) glm::vec3 specular = { 1.f, 1.f, 1.f };			// Ks: Specular Color Reaction [0,1]³
		alignas(16) glm::vec3 transmittance = { 1.f, 1.f, 1.f };	// Tf: Transmission Filter [0,1]³
		alignas(16) glm::vec3 emission = { 0.f, 0.f, 0.f };			// Ke: Emissive Color [0,1]³
		alignas(16) float shininess = 100.0;						// Ns: Specular Highlight [0,1000]
		float ior = 1.f;								// Ni: Optical Density -> Index Of Refraction [0.001,10] (e.g. glass 1.5)
		float dissolve = 1.0f;							// d:  Dissolve Factor -> Opacity [0,1]
	};

	struct EntityModelUniform {
		glm::mat4 transform;
		glm::mat4 normalMat;
	};

}
