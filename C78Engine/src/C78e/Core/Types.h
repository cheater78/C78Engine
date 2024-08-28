#pragma once

#include "Primitives.h"

namespace C78E {

	struct Vertex {
	public:
		glm::vec3 position;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec2 texCoord;
		float texIndex = 0.f;

		Vertex() = default;
		Vertex(glm::vec3 position, glm::vec4 color, glm::vec3 normal, glm::vec2 texCoord, float texIndex)
			: position(position), color(color), normal(normal), texCoord(texCoord), texIndex(texIndex)
		{}
		Vertex(const Vertex& other) = default;
		~Vertex() {}

		bool operator==(const Vertex& other) const {
			return position == other.position && color == other.color && normal == other.normal && texCoord == other.texCoord && texIndex == other.texIndex;
		}

		inline operator Primitive::Vertex() {
			return Primitive::Vertex{position, normal};
		}
	};

}