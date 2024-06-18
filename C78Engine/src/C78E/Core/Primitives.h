#pragma once

#include "C78EPCH.h"

namespace C78E::Primitive {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;

	};

	struct Triangle {
		Vertex vertA;
		Vertex vertB;
		Vertex vertC;

		inline const glm::vec3 center() const {
			return glm::vec3{
				(vertA.position.x + vertB.position.x + vertC.position.x) / 3.f,
				(vertA.position.y + vertB.position.y + vertC.position.y) / 3.f,
				(vertA.position.z + vertB.position.z + vertC.position.z) / 3.f
			};
		}
	};

}
