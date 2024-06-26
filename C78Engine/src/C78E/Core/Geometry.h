#pragma once
#include "Primitives.h"

namespace C78E::Geometry {

	struct Line {
	public:
		Primitive::Vertex vertA{ {0.f, -0.5f, 0.f}, {1.f, 0.f, 0.f} };
		Primitive::Vertex vertB{ {0.f, +0.5f, 0.f}, {1.f, 0.f, 0.f} };

		uint32_t indecies[2] = { 0, 1 };
		
		void* getVertexData() { return this; }
		size_t getVertexSize() const { return 2*sizeof(Primitive::Vertex); }
		void* getIndexData() const { return (void*) &indecies; }
		uint32_t getIndexCount() const { return 2; }

		inline const glm::vec3 center() const { return (vertA.position + vertB.position) / 2.f; }
	private:
	};

	struct Triangle {
	public:
		Primitive::Vertex vertA{ {-0.5f, -0.5f, 0.f}, {0.f, 0.f, -1.f} };
		Primitive::Vertex vertB{ {+0.5f, -0.5f, 0.f}, {0.f, 0.f, -1.f} };
		Primitive::Vertex vertC{ {+0.0f, +0.5f, 0.f}, {0.f, 0.f, -1.f} };

		uint32_t indecies[3] = { 0, 1, 2 };

		Triangle(Primitive::Vertex verta, Primitive::Vertex vertb, Primitive::Vertex vertc) : vertA(verta), vertB(vertc), vertC(vertc) {}
		Triangle(const Triangle& other) = default;
		~Triangle() = default;

		void* getVertexData() { return this; }
		size_t getVertexSize() const { return 3 * sizeof(Primitive::Vertex); }
		void* getIndexData() const { return (void*)&indecies; }
		uint32_t getIndexCount() const { return 3; }

		inline const glm::vec3 center() const { return (vertA.position + vertB.position + vertC.position) / 3.f; }
	};

	struct Quad {
	public:
		Primitive::Vertex vertA{ { -.5f, -.5f, -.5f }, { 0.f, 0.f, -1.f } };
		Primitive::Vertex vertB{ { +.5f, -.5f, -.5f }, { 0.f, 0.f, -1.f } };
		Primitive::Vertex vertC{ { +.5f, +.5f, -.5f }, { 0.f, 0.f, -1.f } };
		Primitive::Vertex vertD{ { -.5f, +.5f, -.5f }, { 0.f, 0.f, -1.f } };

		uint32_t indecies[6] = { 0, 1, 2, 2, 3, 0 };

		void* getVertexData() { return this; }
		size_t getVertexSize() const { return 4 * sizeof(Primitive::Vertex); }
		void* getIndexData() const { return (void*)&indecies; }
		uint32_t getIndexCount() const { return 6; }

		inline const glm::vec3 center() const { return (vertA.position + vertB.position + vertC.position + vertD.position) / 4.f; }
	};

	struct Cube {
	public:
		const Primitive::Vertex vertecies[24] = {
			//Front
			{{ -.5f, -.5f, -.5f }, { 0.f, 0.f, -1.f }},
			{{ +.5f, -.5f, -.5f }, { 0.f, 0.f, -1.f }},
			{{ +.5f, +.5f, -.5f }, { 0.f, 0.f, -1.f }},
			{{ -.5f, +.5f, -.5f }, { 0.f, 0.f, -1.f }},
			//Back
			{{ +.5f, -.5f, +.5f }, { 0.f, 0.f, +1.f }},
			{{ -.5f, -.5f, +.5f }, { 0.f, 0.f, +1.f }},
			{{ -.5f, +.5f, +.5f }, { 0.f, 0.f, +1.f }},
			{{ +.5f, +.5f, +.5f }, { 0.f, 0.f, +1.f }},
			//Right
			{{ +.5f, -.5f, -.5f }, { +1.f, 0.f, 0.f }},
			{{ +.5f, -.5f, +.5f }, { +1.f, 0.f, 0.f }},
			{{ +.5f, +.5f, +.5f }, { +1.f, 0.f, 0.f }},
			{{ +.5f, +.5f, -.5f }, { +1.f, 0.f, 0.f }},
			//Left
			{{ -.5f, -.5f, +.5f }, { -1.f, 0.f, 0.f }},
			{{ -.5f, -.5f, -.5f }, { -1.f, 0.f, 0.f }},
			{{ -.5f, +.5f, -.5f }, { -1.f, 0.f, 0.f }},
			{{ -.5f, +.5f, +.5f }, { -1.f, 0.f, 0.f }},
			//Top
			{{ -.5f, +.5f, -.5f }, { 0.f, +1.f, 0.f }},
			{{ +.5f, +.5f, -.5f }, { 0.f, +1.f, 0.f }},
			{{ +.5f, +.5f, +.5f }, { 0.f, +1.f, 0.f }},
			{{ -.5f, +.5f, +.5f }, { 0.f, +1.f, 0.f }},
			//Bottom
			{{ -.5f, -.5f, +.5f }, { 0.f, -1.f, 0.f }},
			{{ +.5f, -.5f, +.5f }, { 0.f, -1.f, 0.f }},
			{{ +.5f, -.5f, -.5f }, { 0.f, -1.f, 0.f }},
			{{ -.5f, -.5f, -.5f }, { 0.f, -1.f, 0.f }},
		};

		const uint32_t indecies[36] = {
			0,  1,  2,  2,  3,  0,
			4,  5,  6,  6,  7,  4,
			8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		void* getVertexData() { return this; }
		size_t getVertexSize() const { return 24 * sizeof(Primitive::Vertex); }
		void* getIndexData() const { return (void*)&indecies; }
		uint32_t getIndexCount() const { return 36; }
	};

}
