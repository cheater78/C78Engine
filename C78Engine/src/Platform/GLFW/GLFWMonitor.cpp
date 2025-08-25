#include "C78EPCH.h"
#include "GLFWMonitor.h"

namespace C78E {

	GLFWMonitor::GLFWMonitor(GLFWmonitor* monitor)
		: m_Monitor(monitor) {

	}

	MonitorPosition GLFWMonitor::getPosition() const {
		int x, y;
		glfwGetMonitorPos(m_Monitor, &x, &y);
		return MonitorPosition(x, y);
	}

	MonitorSize GLFWMonitor::getSize() const {
		int x, y, dx, dy;
		glfwGetMonitorWorkarea(m_Monitor, &x, &y, &dx, &dy);
		return MonitorSize(dx, dy);
	}

	PhysicalMonitorSizeCM GLFWMonitor::getPhysicalSize() const {
		int dx, dy;
		glfwGetMonitorPhysicalSize(m_Monitor, &dx, &dy);
		return PhysicalMonitorSizeCM(dx / 10.f, dy / 10.f); // mm -> cm
	}

	vec2 GLFWMonitor::getContentScale() const {
		float sx, sy;
		glfwGetMonitorContentScale(m_Monitor, &sx, &sy);
		return vec2(sx, sy);
	}

	std::string GLFWMonitor::getName() const {
		return glfwGetMonitorName(m_Monitor);
	}

	void* GLFWMonitor::getNativeMonitor() {
		return m_Monitor;
	}

}
