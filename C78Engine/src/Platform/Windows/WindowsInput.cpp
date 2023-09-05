#include "C78ePCH.h"
#include "C78e/Core/Input.h"

#include "C78e/Core/Application.h"
#include <GLFW/glfw3.h>

namespace C78e {

	bool Input::isKeyPressed(const KeyCode key)
	{
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::isMouseButtonPressed(const MouseCode button)
	{
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::getMousePosition()
	{
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	void Input::setMousePosition(float x, float y) {
		auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		glfwSetCursorPos(window, static_cast<double>(x), static_cast<double>(y) );
	}

	float Input::getMouseX()
	{
		return getMousePosition().x;
	}

	float Input::getMouseY()
	{
		return getMousePosition().y;
	}

}