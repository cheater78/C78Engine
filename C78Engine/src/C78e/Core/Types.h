#pragma once
#include "C78e.h"


namespace C78e {

	struct Vertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
		float TexIndex;

		// Editor-only
		int EntityID;
	};

	struct AmbientLight {
		glm::vec4 color{};
	};

	struct DirectLight {
		glm::vec3 direction{};
		glm::vec4 color{};
	};

	struct PointLight {
		glm::vec4 position{};
		glm::vec4 color{};
	};

	struct SpotLight {
		glm::vec4 position{};
		glm::vec4 color{};
		alignas(16) float angle;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////                       UniformBuffer Types                      ////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	struct CameraUniform
	{
		glm::mat4 projMat;
		glm::mat4 viewMat;
		glm::mat4 invViewMat{ 1.f };
	};

#define MAX_DIRECT_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10
	struct SceneLightUniform {
		AmbientLight ambientLight{ {1.f, 1.f, 1.f, .05f} };

		DirectLight directLights[MAX_DIRECT_LIGHTS];
		int directLightCount = 0;
		PointLight pointLights[MAX_POINT_LIGHTS];
		int pointLightCount = 0;
		SpotLight spotLights[MAX_SPOT_LIGHTS];
		int spotLightCount = 0;
	};

	struct EntityUniform {
		glm::mat4 transform;
	};

}