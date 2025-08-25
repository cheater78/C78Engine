#pragma once
#include "WindowThread.h"
#include "Monitor.h"

namespace C78E {

	enum class WindowSystemType {
		GLFW,
	};

	class WindowSystem {
	public:
		static Scope<WindowSystem> create(WindowSystemType type = WindowSystemType::GLFW);
	public:
		WindowSystem();
		virtual ~WindowSystem();

		virtual void waitEvents() = 0; // non buisy wait impl!
		virtual void callEvents() = 0;

		virtual void fetchMonitors() = 0;

	public:
		Window& createWindow(const WindowCreateInfo& createInfo);
		bool hasWindows() const;
		void closeAll();
		void freeClosedWindows();
		void callEvent(Event& e);
		const std::vector<Ref<Monitor>>& getMonitors() const;
		Ref<Monitor> getPrimaryMonitor() const;

	protected:
		std::shared_mutex m_WindowThreadAccess;
		std::vector<Scope<WindowThread>> m_WindowThreads; // keeping them Scoped -> makes vector realloct easier, native move

		std::vector<Ref<Monitor>> m_Monitors;
		size_t m_PrimaryMonitorIndex = -1;
	};

}
