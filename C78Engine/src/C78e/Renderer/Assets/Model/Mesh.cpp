#include "C78EPCH.h"
#include "Mesh.h"

namespace C78E {

	Mesh::Mesh()
		: m_Vertecies(createRef<std::vector<Vertex>>()), m_Indicies(createRef<std::vector<uint32_t>>()), m_HasIndicies(true)
	{ }
	Mesh::Mesh(std::vector<Vertex> vertecies, std::vector<uint32_t> indicies)
		: m_Vertecies(createRef<std::vector<Vertex>>(vertecies)), m_Indicies(createRef<std::vector<uint32_t>>(indicies)), m_HasIndicies(true)
	{
	}
	Mesh::Mesh(const char* file)
	{
		//TODO: FileManager
	}
	
	std::vector<Vertex>& Mesh::getVertexData() { return *m_Vertecies; }
	std::vector<uint32_t>& Mesh::getIndexData() { return *m_Indicies; }
}