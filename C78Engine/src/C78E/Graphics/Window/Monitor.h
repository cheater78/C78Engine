#pragma once

namespace C78E {

	using MonitorPosition = ivec2;
	using MonitorSize = uvec2;
	using PhysicalMonitorSizeCM = vec2;

	class Monitor {
	public:
		Monitor() = default;
		virtual ~Monitor() = default;

		virtual MonitorPosition getPosition() const = 0;
		virtual MonitorSize getSize() const = 0;
		virtual PhysicalMonitorSizeCM getPhysicalSize() const = 0;
		virtual vec2 getContentScale() const = 0;
		virtual std::string getName() const = 0;

		virtual void* getNativeMonitor() = 0;
	protected:
	};

}
