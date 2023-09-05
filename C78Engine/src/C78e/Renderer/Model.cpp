#include "C78ePCH.h"
#include "Model.h"

namespace C78e {

	Model::Model()
		: m_Vertecies(), m_Indicies(), m_HasIndicies(true)
	{ }
	Model::Model(std::vector<Vertex> vertecies, std::vector<uint32_t> indicies)
	{
		setData(vertecies, indicies);
	}
	Model::Model(const char* file)
	{
		//TODO: FileManager
	}
	

	void Model::setData(std::vector<Vertex>& vertecies, std::vector<uint32_t>& indicies) {
		setData(vertecies.data(), vertecies.size(), indicies.data(), indicies.size());
	}

	void Model::setData(void* vertecies, uint32_t vertexCount, void* indicies, uint32_t indexCount) {
		m_Vertecies.resize(vertexCount * sizeof(Vertex));
		memcpy_s(m_Vertecies.data(), m_Vertecies.size() * sizeof(Vertex), vertecies, vertexCount * sizeof(Vertex));
		m_HasIndicies = indexCount;
		if (m_HasIndicies) {
			m_Indicies.resize(indexCount * sizeof(uint32_t));
			memcpy_s(m_Vertecies.data(), m_Indicies.size() * sizeof(uint32_t), indicies, indexCount * sizeof(uint32_t));
		}
	}
	
	std::vector<Vertex>& Model::getVertexData() { return m_Vertecies; }
	std::vector<uint32_t>& Model::getIndexData() { return m_Indicies; }
}