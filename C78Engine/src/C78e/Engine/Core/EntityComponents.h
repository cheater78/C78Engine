#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>

#include <entt/entt.hpp>

#include "Engine/Util/types.h"

#include "UUID.h"

#include <string>
#include <vector>

namespace C78e {

	struct IDComponent {
	public:
		UUID id;

		IDComponent();
		IDComponent(UUID uuid);
		IDComponent(const IDComponent&) = default;

	};

	struct TagComponent {
	public:
		std::string tag;

		TagComponent();
		TagComponent(const std::string atag);
		TagComponent(const TagComponent&) = default;

	};

	struct TransformComponent {
	public:
		glm::vec3 translation{ 0.f, 0.f, 0.f };
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{ 0.f, 0.f, 0.f };

		glm::mat4 mat4();
		glm::mat3 normalMat();
	};

	struct CameraComponent {
	public:
		glm::mat4 projMat{ 1.f };
	};

	struct MeshComponent {
	public:

		std::vector<Mesh::Vertex> vertecies;
		std::vector<uint32_t> indecies;

		MeshComponent() = delete;
		MeshComponent(const MeshComponent& other) = default;
		MeshComponent(std::vector<Mesh::Vertex> vertecies, std::vector<uint32_t> indecies = {});
		MeshComponent(std::string file);

		bool hasIndexBuffer() { return indecies.size(); }
	private:
		
	};

	struct MaterialComponent {
	public:
		std::string file;
		int width;
		int height;
		int channels;

		std::vector<char> data;

		MaterialComponent() = delete;
		MaterialComponent(const RawImage& image);
	};

	struct PointLightComponent {
		glm::vec3 color; // rgb
		float intensity;
		float radius;

		PointLightComponent(glm::vec3 color, float intensity, float radius);

	};

	struct SpotLightComponent {
		glm::vec3 color; // rgb
		glm::vec3 direction;
		float intensity;
		float angle;
	};

}