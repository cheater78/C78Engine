#pragma once
#include <C78E/Core/Types.h>

namespace C78E {

	class Mesh
	{
	public:
		Mesh();
		Mesh(const Mesh&) = default;
		Mesh(std::vector<Vertex> vertecies, std::vector<uint32_t> indicies);
		Mesh(std::string filepath);

		std::vector<Vertex>& getVertexData();
		std::vector<uint32_t>& getIndexData();

		std::string getName() { return m_Name; }
		void setName(std::string name) { m_Name = name; }
	private:
		std::string m_Name;
		Ref<std::vector<Vertex>> m_Vertecies;
		Ref<std::vector<uint32_t>> m_Indicies;

		bool m_HasIndicies = false;
	};
}