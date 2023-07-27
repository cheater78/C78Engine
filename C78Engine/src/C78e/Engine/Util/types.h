#pragma once
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace C78e {

	struct Mesh {
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

		std::vector<Vertex> vertecies;
		std::vector<uint32_t> indecies;
	};

	

	struct RawImage {
		void* data;
		int width;
		int height;
		int channels;
		std::string file;
	};

}