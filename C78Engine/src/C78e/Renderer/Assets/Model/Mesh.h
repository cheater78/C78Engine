#pragma once
#include <C78e/Core/Types.h>

namespace C78e {

	class Mesh
	{
	public:
		Mesh();
		Mesh(std::vector<Vertex> vertecies, std::vector<uint32_t> indicies);
		Mesh(const char* file);

		//void setData(std::vector<Vertex>& vertecies, std::vector<uint32_t>& indicies = std::vector<uint32_t>());
		//void setData(void* vertecies, uint32_t vertexCount, void* indicies = nullptr, uint32_t indexCount = 0);

		std::vector<Vertex>& getVertexData();
		std::vector<uint32_t>& getIndexData();


	private:
		Ref<std::vector<Vertex>> m_Vertecies;
		Ref<std::vector<uint32_t>> m_Indicies;

		bool m_HasIndicies = false;
	};
}