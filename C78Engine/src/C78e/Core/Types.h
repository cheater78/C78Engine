#pragma once
#include "C78EPCH.h"

namespace C78E {

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
		glm::vec4 color{ 1.f, 1.f, 1.f, .0f };
	};

	struct DirectLight {
		glm::vec3 direction{0.f, -1.f, 0.f};
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
		float angle = 30.f/180.f*glm::pi<float>();
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
		AmbientLight ambientLight{ {1.f, 1.f, 1.f, .1f} };

		DirectLight directLights[MAX_DIRECT_LIGHTS]{};
		int directLightCount = 0;
		alignas(16) PointLight pointLights[MAX_POINT_LIGHTS]{};
		int pointLightCount = 0;
		alignas(16) SpotLight spotLights[MAX_SPOT_LIGHTS]{};
		int spotLightCount = 0;
	};

	struct EntityUniform {
		glm::mat4 transform;
		glm::mat4 normalMat;
	};

}