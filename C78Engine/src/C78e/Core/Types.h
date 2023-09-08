#pragma once
#include "C78ePCH.h"

namespace C78e {

	struct Vertex {
	public:
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
		float TexIndex;

		Vertex() = default;
		Vertex(glm::vec3 position, glm::vec4 color, glm::vec3 normal, glm::vec2 texCoord, float texIndex)
			: Position(position), Color(color), Normal(normal), TexCoord(texCoord), TexIndex(texIndex)
		{}
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
		glm::mat4 projMat{ 1.f };
		glm::mat4 viewMat{ 1.f };
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