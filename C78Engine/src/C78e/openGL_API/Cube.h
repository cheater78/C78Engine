#pragma once

#include "GeoHelp.h"
#include "Renderer.h"
#include <array>


class Cube {
public:
	Cube(float x = 0.0f, float y = 0.0f, float z = 0.0f, float dx = 1.0f, float dy = 1.0f, float dz = 1.0f, float pitch = 0.0f, float yaw = 0.0f, float roll = 0.0f);
	Cube(glm::vec3 pos, glm::vec3 dim, glm::vec3 ang);
	virtual ~Cube();

	std::array<float, 56> vertecies();
	std::array<unsigned int, 36> indecies();

private:
	glm::vec3 m_Pos;
	glm::vec3 m_Dim;
	glm::vec3 m_Ang;

};