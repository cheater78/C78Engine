#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

#include <string>
#include <list>

namespace C78e {
	
	struct Vertex
	{
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec1 texID{};
		glm::vec2 uv{};

		Vertex() = default;
		Vertex& operator=(const Vertex&) = default;

		bool operator==(const Vertex& other) const {
			return true
				&& position == other.position
				&& color == other.color
				&& normal == other.normal
				&& texID == other.texID
				&& uv == other.uv;
		}
	};

	struct ShaderSet {
		std::string vertSh;
		std::string fragSh;
	};

	struct SimplePushConstantData {
		glm::mat4 modelMat{ 1.f };
		glm::mat4 normalMat{ 1.f };
	};

	struct GlobalUBO {
		glm::mat4 projMat;
		glm::mat4 viewMat;
		glm::mat4 invViewMat{ 1.f };
	};

	struct DirectLight {
		glm::vec4 direction{};
		glm::vec4 color{};
		alignas(16) float intensity{0.f};
	};

	struct PointLight{
		glm::vec4 position{};
		glm::vec4 color{};
		alignas(16) float radius{1.f};
	};

#define MAX_DIRECT_LIGHTS 10
#define MAX_POINT_LIGHTS 10
	struct LightUBO {
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .05f };
		DirectLight directLights[MAX_DIRECT_LIGHTS];
		int directLightCount = 0;
		PointLight pointLights[MAX_POINT_LIGHTS];
		int pointLightCount = 0;
	};


	class VulkanModel;
	class Scene;
	struct RenderTask {
		int frameIndex;
		VkCommandBuffer commandBuffer;
		Scene* scene;
		std::list<uint64_t,VulkanModel>& models;
	};

	struct FrameInfo {
		float frameTime;
	};
}