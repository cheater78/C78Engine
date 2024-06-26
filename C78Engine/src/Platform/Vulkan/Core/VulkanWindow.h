#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace C78E {

	class VulkanWindow {
	public:
		VulkanWindow(uint32_t w, uint32_t h, std::string name);
		~VulkanWindow();

		VulkanWindow(const VulkanWindow&) = delete;
		VulkanWindow& operator=(const VulkanWindow&) = delete;

		bool stopped();

		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

		VkExtent2D getExtent() { return { m_Width, m_Height }; };

		bool wasWindowResized() { return m_FramebufferResized; };

		void resetWindowResizedFlag() { m_FramebufferResized = false; };

		GLFWwindow* getGLFWwindow() const { return m_Window; }

	private:

		GLFWwindow* m_Window;
		std::string m_WinName;
		uint32_t m_Width;
		uint32_t m_Height;
		bool m_FramebufferResized = false;

		void init();
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	};

}