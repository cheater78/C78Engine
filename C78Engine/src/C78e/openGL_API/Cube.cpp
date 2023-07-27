#include "Cube.h"
#include "glm/glm.hpp"

Cube::Cube(float x, float y, float z, float dx, float dy, float dz, float pitch, float yaw, float roll) {
	m_Pos = glm::vec3(x, y, z);
	m_Dim = glm::vec3(dx, dy, dz);
	m_Ang = glm::vec3(pitch, yaw, roll);
}

Cube::Cube(glm::vec3 pos, glm::vec3 dim, glm::vec3 ang) : m_Pos(pos), m_Dim(dim), m_Ang(ang) {
	
}

Cube::~Cube() {

}

std::array<float, 56> Cube::vertecies() {

	std::array<float, 56> vb{
		m_Pos.x - 0.5f * m_Dim.x, m_Pos.y - 0.5f * m_Dim.y,m_Pos.z + 0.5f * m_Dim.z, 1.0f, 1.0f, 1.0f, 1.0f,
		m_Pos.x + 0.5f * m_Dim.x, m_Pos.y - 0.5f * m_Dim.y,m_Pos.z + 0.5f * m_Dim.z, 1.0f, 0.0f, 0.0f, 1.0f,
		m_Pos.x + 0.5f * m_Dim.x, m_Pos.y + 0.5f * m_Dim.y,m_Pos.z + 0.5f * m_Dim.z, 0.0f, 1.0f, 0.0f, 1.0f,
		m_Pos.x - 0.5f * m_Dim.x, m_Pos.y + 0.5f * m_Dim.y,m_Pos.z + 0.5f * m_Dim.z, 0.0f, 0.0f, 1.0f, 1.0f,
																									   
		m_Pos.x + 0.5f * m_Dim.x, m_Pos.y + 0.5f * m_Dim.y,m_Pos.z - 0.5f * m_Dim.z, 1.0f, 1.0f, 1.0f, 1.0f,
		m_Pos.x - 0.5f * m_Dim.x, m_Pos.y + 0.5f * m_Dim.y,m_Pos.z - 0.5f * m_Dim.z, 1.0f, 0.0f, 0.0f, 1.0f,
		m_Pos.x - 0.5f * m_Dim.x, m_Pos.y - 0.5f * m_Dim.y,m_Pos.z - 0.5f * m_Dim.z, 0.0f, 1.0f, 0.0f, 1.0f,
		m_Pos.x + 0.5f * m_Dim.x, m_Pos.y - 0.5f * m_Dim.y,m_Pos.z - 0.5f * m_Dim.z, 0.0f, 0.0f, 1.0f, 1.0f
	};
	
	return vb;
}


std::array<unsigned int, 36> Cube::indecies() {

	std::array<unsigned int, 36> ib{
		0, 1, 2, 2, 3, 0,
		7, 6, 5, 5, 4, 7,
		1, 7, 4, 4, 2, 1,
		6, 0, 3, 3, 5, 6,
		3, 2, 4, 4, 5, 3,
		1, 0, 6, 6, 7, 1
	};

	return ib;
}