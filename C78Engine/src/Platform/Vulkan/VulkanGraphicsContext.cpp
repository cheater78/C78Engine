#include "C78EPCH.h"
#include "VulkanGraphicsContext.h"

namespace C78E {

	VulkanGraphicsContext::VulkanGraphicsContext(Window& window)
		: m_Window(window) {
	}

	void VulkanGraphicsContext::init() {
	}

	void VulkanGraphicsContext::swapBuffers() {

	}

	GLFWwindow* VulkanGraphicsContext::getGLFWwindow() const { return static_cast<GLFWwindow*>(m_Window.getNativeWindow()); }
}
