#pragma once

#include "C78EPCH.h"

namespace C78E::Primitive {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;

	};

	struct VertexColor {
		glm::vec4 color;
	};

	struct VertexTexture {
		glm::vec2 textureCoordinate;
		alignas(16) float textureIndex; //TODO: check align
	};

	struct CubeMap {
	public:
		const glm::vec3 vertecies[8] = {
			//Front
			{ -1.f, -1.f, +1.f },
			{ +1.f, -1.f, +1.f },
			{ +1.f, +1.f, +1.f },
			{ -1.f, +1.f, +1.f },
			//Back
			{ +1.f, -1.f, -1.f },
			{ -1.f, -1.f, -1.f },
			{ -1.f, +1.f, -1.f },
			{ +1.f, +1.f, -1.f }
		};

		const uint32_t indecies[36] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			1, 4, 7, 7, 2, 1,
			5, 0, 2, 2, 6, 5,
			3, 2, 7, 7, 6, 3,
			5, 4, 1, 1, 0, 5
		};

		void* getVertexData() { return (void*)&vertecies; }
		size_t getVertexSize() const { return 8 * sizeof(glm::vec3); }
		void* getIndexData() const { return (void*)&indecies; }
		uint32_t getIndexCount() const { return 36; }
	};
}
