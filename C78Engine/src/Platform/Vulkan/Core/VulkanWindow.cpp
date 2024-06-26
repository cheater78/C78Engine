#include "C78EPCH.h"
#include "vulkanWindow.h"

namespace C78E {

	VulkanWindow::VulkanWindow(uint32_t w, uint32_t h, std::string name) : m_Width(w), m_Height(h), m_WinName(name) {
		init();
	}

	VulkanWindow::~VulkanWindow() {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void VulkanWindow::init() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_WinName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);
	}
	
	void VulkanWindow::framebufferResizeCallback(GLFWwindow* window, int m_Width, int m_Height) {
		auto vulkanWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
		vulkanWindow->m_FramebufferResized = true;
		vulkanWindow->m_Width = m_Width;
		vulkanWindow->m_Height = m_Height;
	}
	

	void VulkanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create surface");
		}
	}

	bool VulkanWindow::stopped() {
		return glfwWindowShouldClose(m_Window);
	}
}