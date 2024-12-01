#include "C78EPCH.h"

#ifdef C78E_PLATFORM_GLFW
#include <C78E/Core/Input/Input.h>
#include <C78E/Core/Application/Application.h>

namespace C78E::Input {

	bool isKeyPressed(const KeyCode key) {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool isMouseButtonPressed(const MouseCode button) {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 getMousePosition() {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	void setMousePosition(float x, float y) {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		glfwSetCursorPos(window, static_cast<double>(x), static_cast<double>(y));
	}

}

#endif // C78E_PLATFORM_GLFW