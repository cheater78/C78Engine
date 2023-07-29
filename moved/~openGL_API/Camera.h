#pragma once

#include "Shader.h"
#include "DebugGL.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer;


class Camera {

public:
	Camera();
	virtual ~Camera();

	void rotate(float yaw, float pitch);
	void translate(glm::vec3 v);

	glm::vec3 getLookAt();
	glm::mat4 getProjView(glm::vec2 camDim);
	glm::vec3 getPos() { return mPos; };

	void handleInputs(Renderer& renderer, float delta);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
private:
	glm::vec3 mPos;
	glm::vec2 mAng;
	float mFov;
	float mMouseSens;
	bool mWinActive;

};