#include "C78EPCH.h"
#include "GLFWWindowSystem.h"

#include <C78E/Graphics/API/GraphicsInstance.h>

namespace C78E {

	void GLFWErrorCallback(int error, const char* description) {
		C78E_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	GLFWWindowSystem::GLFWWindowSystem()
		: WindowSystem() {
		C78E_CORE_TRACE("GLFWWindowSystem::GLFWWindowSystem: Creating GLFWWindowSystem..");

		// GLFW instance
		const int glfwInitResult = glfwInit();
		C78E_CORE_VALIDATE(glfwInitResult, return,
			"GLFWWindowSystem::GLFWWindowSystem: Could not initialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);

		fetchMonitors();

		GraphicsInstance::create(API::Vulkan); //TODO:
	}

	GLFWWindowSystem::~GLFWWindowSystem() {
		C78E_CORE_TRACE("GLFWWindowSystem::GLFWWindowSystem: Shutting down GLFWWindowSystem..");
		glfwTerminate();
	}

	void GLFWWindowSystem::waitEvents() {
		glfwWaitEvents();
	}

	void GLFWWindowSystem::callEvents() {
		glfwPollEvents();
	}

	void GLFWWindowSystem::fetchMonitors() {
		C78E_CORE_TRACE("GLFWWindowSystem::fetchMonitors: Fetching Monitors...");
		int monitorCount = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		C78E_CORE_TRACE("GLFWWindowSystem::fetchMonitors: {} found.", monitorCount);
		for (size_t i = 0; i < static_cast<size_t>(monitorCount); i++) {
			m_Monitors.emplace_back(createRef<GLFWMonitor>(monitors[i]));
			if (primaryMonitor == monitors[i]) {
				m_PrimaryMonitorIndex = i;
				C78E_CORE_TRACE("GLFWWindowSystem::fetchMonitors: [{}] {} [Primary]", i, m_Monitors.back()->getName());
			}
			else {
				C78E_CORE_TRACE("GLFWWindowSystem::fetchMonitors: [{}] {}", i, m_Monitors.back()->getName());
			}

		}
		C78E_CORE_VALIDATE(m_PrimaryMonitorIndex != -1, "GLFWWindowSystem::fetchMonitors: m_PrimaryMonitorIndex was not set, this will be bad at some point!");
	}

}
