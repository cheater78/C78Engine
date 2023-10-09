#include "C78EPCH.h"
#include <C78E/Core/Types.h>
#include "C78E/Renderer/Assets/Model/Mesh.h"

namespace C78E {
	namespace GenericShape {
		static const glm::vec4 WHITE = glm::vec4(1.f, 1.f, 1.f, 1.f);
		class Triangle {
		public:

			static std::vector<Vertex> getVertexData(glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f), uint32_t texID = 0) {
				return std::vector<Vertex>{
					Vertex(glm::vec3{ -.5f, -(glm::sqrt(3) / 6.f), 0.f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 0.0f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, -(glm::sqrt(3) / 6.f), 0.f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 1.0f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.0f, +(glm::sqrt(3) / 3.f), 0.f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 0.5f, 1.f }, static_cast<float>(texID))
				};
			}
			static std::vector<uint32_t> getIndexData() { return std::vector<uint32_t>{ 0, 1, 2 }; };
			static Mesh getMesh(glm::vec4 color, uint32_t texID) { return Mesh(getVertexData(color, texID), getIndexData()); }
		private:
		};

		class Quad {
		public:

			static std::vector<Vertex> getVertexData(glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f), uint32_t texID = 0) {
				return std::vector<Vertex>{
					Vertex(glm::vec3{ -.5f, -.5f, 0.f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 0.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, -.5f, 0.f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 1.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, +.5f, 0.f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 1.f, 1.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, +.5f, 0.f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 0.f, 1.f }, static_cast<float>(texID)),
				};
			}
			static std::vector<uint32_t> getIndexData() { return std::vector<uint32_t>{ 0, 1, 2, 2, 3, 0 }; };
			static Mesh getMesh(glm::vec4 color, uint32_t texID) { return Mesh(getVertexData(color, texID), getIndexData()); }
		private:
		};

		class Cube {
		public:

			static std::vector<Vertex> getVertexData(glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f), uint32_t texID = 0) {
				return std::vector<Vertex>{
					/*
					LFR
					TBb
					*/

					//Front
					Vertex(glm::vec3{ -.5f, -.5f, +.5f }, color, glm::vec3{ 0.f, 0.f, +1.f }, glm::vec2{ 0.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, -.5f, +.5f }, color, glm::vec3{ 0.f, 0.f, +1.f }, glm::vec2{ 1.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, +.5f, +.5f }, color, glm::vec3{ 0.f, 0.f, +1.f }, glm::vec2{ 1.f, 1.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, +.5f, +.5f }, color, glm::vec3{ 0.f, 0.f, +1.f }, glm::vec2{ 0.f, 1.f }, static_cast<float>(texID)),
					//Back
					Vertex(glm::vec3{ +.5f, -.5f, -.5f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 0.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, -.5f, -.5f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 1.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, +.5f, -.5f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 1.f, 1.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, +.5f, -.5f }, color, glm::vec3{ 0.f, 0.f, -1.f }, glm::vec2{ 0.f, 1.f }, static_cast<float>(texID)),
					//Right
					Vertex(glm::vec3{ +.5f, -.5f, +.5f }, color, glm::vec3{ +1.f, 0.f, 0.f }, glm::vec2{ 0.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, -.5f, -.5f }, color, glm::vec3{ +1.f, 0.f, 0.f }, glm::vec2{ 1.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, +.5f, -.5f }, color, glm::vec3{ +1.f, 0.f, 0.f }, glm::vec2{ 1.f, 1.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, +.5f, +.5f }, color, glm::vec3{ +1.f, 0.f, 0.f }, glm::vec2{ 0.f, 1.f }, static_cast<float>(texID)),
					//Left
					Vertex(glm::vec3{ -.5f, -.5f, -.5f }, color, glm::vec3{ -1.f, 0.f, 0.f }, glm::vec2{ 0.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, -.5f, +.5f }, color, glm::vec3{ -1.f, 0.f, 0.f }, glm::vec2{ 1.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, +.5f, +.5f }, color, glm::vec3{ -1.f, 0.f, 0.f }, glm::vec2{ 1.f, 1.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, +.5f, -.5f }, color, glm::vec3{ -1.f, 0.f, 0.f }, glm::vec2{ 0.f, 1.f }, static_cast<float>(texID)),
					//Top
					Vertex(glm::vec3{ -.5f, +.5f, +.5f }, color, glm::vec3{ 0.f, +1.f, 0.f }, glm::vec2{ 0.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, +.5f, +.5f }, color, glm::vec3{ 0.f, +1.f, 0.f }, glm::vec2{ 1.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, +.5f, -.5f }, color, glm::vec3{ 0.f, +1.f, 0.f }, glm::vec2{ 1.f, 1.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, +.5f, -.5f }, color, glm::vec3{ 0.f, +1.f, 0.f }, glm::vec2{ 0.f, 1.f }, static_cast<float>(texID)),
					//bottom
					Vertex(glm::vec3{ -.5f, -.5f, -.5f }, color, glm::vec3{ 0.f, -1.f, 0.f }, glm::vec2{ 0.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, -.5f, -.5f }, color, glm::vec3{ 0.f, -1.f, 0.f }, glm::vec2{ 1.f, 0.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ +.5f, -.5f, +.5f }, color, glm::vec3{ 0.f, -1.f, 0.f }, glm::vec2{ 1.f, 1.f }, static_cast<float>(texID)),
					Vertex(glm::vec3{ -.5f, -.5f, +.5f }, color, glm::vec3{ 0.f, -1.f, 0.f }, glm::vec2{ 0.f, 1.f }, static_cast<float>(texID))
				};
			}
			static std::vector<uint32_t> getIndexData() {
				return std::vector<uint32_t>{
					 0,  1,  2,  2,  3,  0,
					 4,  5,  6,  6,  7,  4,
					 8,  9, 10, 10, 11,  8,
					12, 13, 14, 14, 15, 12,
					16, 17, 18, 18, 19, 16,
					20, 21, 22, 22, 23, 20
				};
			};
			static Mesh getMesh(glm::vec4 color, uint32_t texID) { return Mesh(getVertexData(color, texID), getIndexData()); }
		private:
		};


	}
}