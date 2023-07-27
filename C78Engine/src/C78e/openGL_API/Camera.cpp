#include "Camera.h"
#include <math.h>
#include "Renderer.h"


Camera::Camera()
{
	mPos = glm::vec3(0.0f,0.0f,0.0f);
	mAng = glm::vec2(0.0f, 0.0f);
	mFov = 90.0f;
	mMouseSens = 0.1f;
	mWinActive = true;
	
}

Camera::~Camera()
{

}


void Camera::rotate(float yaw, float pitch) {
	mAng.x += yaw * mMouseSens;
	mAng.y -= pitch * mMouseSens;
	if (mAng.x < 0) mAng.x = 360;
	if (mAng.x > 360) mAng.x = 0;

	if (mAng.y < -89.99f) mAng.y = -89.99f;
	if (mAng.y > 89.99f) mAng.y = 89.99f;
	
}

void Camera::translate(glm::vec3 v) {
	mPos += v;
}

glm::mat4 Camera::getProjView(glm::vec2 winDim)
{

	glm::mat4 view = glm::lookAt(mPos, mPos + getLookAt(), up);

	glm::mat4 proj = glm::perspective(glm::radians(mFov), winDim.x / winDim.y, 0.01f, 1000.0f);

	return proj * view;
}


glm::vec3 Camera::getLookAt() {
	glm::vec3 lookat;
	lookat.x = cos(glm::radians(mAng.y)) * cos(glm::radians(mAng.x));
	lookat.y = sin(glm::radians(mAng.y));
	lookat.z = cos(glm::radians(mAng.y)) * sin(glm::radians(mAng.x));
	lookat = glm::normalize(lookat);
	return lookat;
}

void Camera::handleInputs(Renderer& renderer, float delta) {

	if (mWinActive && glfwGetInputMode(renderer.getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		//MOUSE MOVEMENT
		double xpos, ypos;
		glfwGetCursorPos(renderer.getWindow(), &xpos, &ypos);
		rotate((xpos - (renderer.getWinDim().x / 2)), (ypos - (renderer.getWinDim().y / 2)));
		glfwSetCursorPos(renderer.getWindow(), renderer.getWinDim().x / 2, renderer.getWinDim().y / 2);

		//KEYBOARD PRESSES
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
			translate(-1.0f * delta * glm::normalize(glm::cross(glm::cross(getLookAt(), up), up) * 10000.f));
		}
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
			translate(1.0f * delta * glm::normalize(glm::cross(glm::cross(getLookAt(), up), up) * 10000.f));
		}
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
			translate(1.0f * delta * glm::normalize(glm::cross(getLookAt(), up) * 10000.f));
		}
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
			translate(-1.0f * delta * glm::normalize(glm::cross(getLookAt(), up) * 10000.f));
		}
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
			translate(1.0f * delta * up);
		}
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			translate(-1.0f * delta * up);
		}
	}

	if (glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS && mWinActive == true && glfwGetInputMode(renderer.getWindow(), GLFW_CURSOR) != GLFW_CURSOR_NORMAL) {
		mWinActive = false;
	}
	if (glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) == GLFW_RELEASE && mWinActive == false) {
		if (glfwGetInputMode(renderer.getWindow(), GLFW_CURSOR) != GLFW_CURSOR_NORMAL) {
			glfwSetInputMode(renderer.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(renderer.getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		}
	}

	if (glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS && mWinActive == false && glfwGetInputMode(renderer.getWindow(), GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
		mWinActive = true;
	}
	if (glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) == GLFW_RELEASE && mWinActive == true) {
		glfwSetCursorPos(renderer.getWindow(), renderer.getWinDim().x / 2, renderer.getWinDim().y / 2);
		if (glfwGetInputMode(renderer.getWindow(), GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(renderer.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(renderer.getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
	}

}