#pragma once
#include <C78E/Graphics/Window/Monitor.h>

namespace C78E {

	class GLFWMonitor : public Monitor {
	public:
		GLFWMonitor(GLFWmonitor* monitor);
		virtual ~GLFWMonitor() = default;

		virtual MonitorPosition getPosition() const override;
		virtual MonitorSize getSize() const override;
		virtual PhysicalMonitorSizeCM getPhysicalSize() const override;
		virtual vec2 getContentScale() const override;
		virtual std::string getName() const override;

		virtual void* getNativeMonitor() override;
	protected:
		GLFWmonitor* m_Monitor;
	};

}
