#include "hid.h"

#include "C78e/engine.h"
using namespace C78e;

#include <limits>

void KeyboardMovementController::moveInYXZ( GLFWwindow* window, float dt, Scene& scene) {

	Entity cam = scene.getActiveCam();
	if (!cam.hasComponent<CameraComponent>()) throw std::runtime_error("Camera is invalid");

	TransformComponent& obj = cam.getComponent<TransformComponent>();

	//get Window Dims
	glm::i32vec2 winDim{ 0, 0 };
	glfwGetWindowSize(window, &winDim.x, &winDim.y);
	
	//get Mouse Pos
	glm::dvec2 mousePos{0.f, 0.f};
	glfwGetCursorPos(window, &mousePos.x, &mousePos.y);

	//reset Mouse Pos to the middle of the window
	glfwSetCursorPos(window, winDim.x / 2, winDim.y / 2);

	//make Mouse Pos relative to the middle of the window
	mousePos -= winDim / 2;

	// apply rotation to the obj
	obj.rotation.y += static_cast<float>(mousePos.x) * dt * mouseSense;
	obj.rotation.x += -static_cast<float>(mousePos.y) * dt * mouseSense;

    // limit pitch values between about +/- 85ish degrees
    obj.rotation.x = glm::clamp(obj.rotation.x, -1.5f, 1.5f);
    obj.rotation.y = glm::mod(obj.rotation.y, glm::two_pi<float>());


	// construct directional normalized vec3 for the direction the obj is looking in
	const glm::vec3 lookat = glm::normalize(glm::vec3{sin(obj.rotation.y), .0f, cos(obj.rotation.y)});
	//hardCode up is up
	const glm::vec3 upDir{ 0.f, -1.f, 0.f };
	// construct normalized moving vec3s
	const glm::vec3 rightDir = glm::normalize(glm::cross(lookat, upDir));
	const glm::vec3 forwardDir = glm::normalize(glm::cross(-rightDir, upDir));
    

    glm::vec3 moveDir{ 0.f };
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        obj.translation += moveSpeed * dt * glm::normalize(moveDir);
    }
}

void KeyboardMovementController::handleInputs(GLFWwindow* window, float dt, Scene& scene) {

	if (winFocused && glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		moveInYXZ(window, dt, scene);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && winFocused == true && glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL) {
		winFocused = false;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && winFocused == false) {
		if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && winFocused == false && glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
		winFocused = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && winFocused == true) {
		//get Window Dims
		glm::i32vec2 winDim{ 0, 0 };
		glfwGetWindowSize(window, &winDim.x, &winDim.y);
		glfwSetCursorPos(window, winDim.x / 2, winDim.y / 2);
		if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
	}

}