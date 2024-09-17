#include "C78EPCH.h"

#ifdef C78_PLATFORM_GLFW
#include <C78E/Core/Input.h>
#include <C78E/Core/Application.h>

namespace C78E {

	bool Input::isKeyPressed(const KeyCode key) {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::isMouseButtonPressed(const MouseCode button) {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::getMousePosition() {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	void Input::setMousePosition(float x, float y) {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		glfwSetCursorPos(window, static_cast<double>(x), static_cast<double>(y));
	}

	float Input::getMouseX() {
		return getMousePosition().x;
	}

	float Input::getMouseY() {
		return getMousePosition().y;
	}

}

#endif // C78_PLATFORM_GLFW