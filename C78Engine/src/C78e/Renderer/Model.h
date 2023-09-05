#pragma once
#include <C78e/Core/Types.h>

namespace C78e {

	class Model
	{
	public:
		Model();
		Model(std::vector<Vertex> vertecies, std::vector<uint32_t> indicies);
		Model(const char* file);

		void setData(std::vector<Vertex>& vertecies, std::vector<uint32_t>& indicies = std::vector<uint32_t>());
		void setData(void* vertecies, uint32_t vertexCount, void* indicies = nullptr, uint32_t indexCount = 0);

		std::vector<Vertex>& getVertexData();
		std::vector<uint32_t>& getIndexData();


	private:
		std::vector<Vertex> m_Vertecies{};
		std::vector<uint32_t> m_Indicies{};

		bool m_HasIndicies = false;
	};
}