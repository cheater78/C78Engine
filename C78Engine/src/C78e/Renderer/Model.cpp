#include "C78ePCH.h"
#include "Model.h"

namespace C78e {

	Model::Model()
		: m_Vertecies(createRef<std::vector<Vertex>>()), m_Indicies(createRef<std::vector<uint32_t>>()), m_HasIndicies(true)
	{ }
	Model::Model(std::vector<Vertex> vertecies, std::vector<uint32_t> indicies)
		: m_Vertecies(createRef<std::vector<Vertex>>(vertecies)), m_Indicies(createRef<std::vector<uint32_t>>(indicies)), m_HasIndicies(true)
	{
	}
	Model::Model(const char* file)
	{
		//TODO: FileManager
	}
	

	
	
	std::vector<Vertex>& Model::getVertexData() { return *m_Vertecies; }
	std::vector<uint32_t>& Model::getIndexData() { return *m_Indicies; }
}