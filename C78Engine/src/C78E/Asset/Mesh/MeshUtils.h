#pragma once

#include "C78EPCH.h"


#include "Mesh.h"

namespace C78E {

	struct Ray {
		glm::vec3 origin{ 0.f, 0.f, 0.f };
		glm::vec3 direction{ 0.f, -1.f, 0.f };
	};

	struct BoundingBox {
	public:
		glm::vec3 min = glm::vec3{ 1.f, 1.f, 1.f } * -std::numeric_limits<float>::infinity();
		glm::vec3 max = glm::vec3{ 1.f, 1.f, 1.f } * +std::numeric_limits<float>::infinity();
		

		BoundingBox() = default;
		BoundingBox(const BoundingBox& other) = default;
		BoundingBox(const Mesh& mesh) { growToInclude(mesh); }
		~BoundingBox() = default;

		void growToInclude(glm::vec3 point) {
			min = glm::min<glm::vec3>(point, min);
			max = glm::max<glm::vec3>(point, max);
		}

		void growToInclude(Mesh::Position position) {
			growToInclude(position);
		}

		void growToInclude(Primitive::Triangle triangle) {
			growToInclude(triangle.vertA);
			growToInclude(triangle.vertA);
			growToInclude(triangle.vertA);
		}

		void growToInclude(const Mesh& mesh) {
			auto triangles = mesh.getTriangles();

			for (auto& triangle : triangles)
				growToInclude(triangle);
		}

		inline const glm::vec3 center() const { return (min + max) * 0.5f; }
		inline const glm::vec3 size() const { return glm::abs<glm::vec3>(max - min); }
	};


}
