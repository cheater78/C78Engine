#include "C78EPCH.h"
#include "WindowSystem.h"

#include <Platform/GLFW/GLFWWindowSystem.h>

namespace C78E {
	
	Scope<WindowSystem> WindowSystem::create(WindowSystemType type) {
		switch (type) {
		case WindowSystemType::GLFW:
			return createScope<GLFWWindowSystem>();
		default:
			C78E_CORE_ASSERT(false, "WindowSystem::create: WindowSystemType not supported!");
			return nullptr;
		}
	}

	WindowSystem::WindowSystem() {
	}

	WindowSystem::~WindowSystem() {
	}

	Window& WindowSystem::createWindow(const WindowCreateInfo& createInfo) {
		std::unique_lock lock(m_WindowThreadAccess);
		return m_WindowThreads.emplace_back(createScope<WindowThread>(createInfo))->getWindow();
	}

	bool WindowSystem::hasWindows() const {
		return !m_WindowThreads.empty();
	}

	void WindowSystem::closeAll() {
		std::shared_lock lock(m_WindowThreadAccess); // just shared lock -> let windows complete their thing, then close
		for (Scope<WindowThread>& wt : m_WindowThreads) {
			wt->getWindow().close();
		}
	}

	void WindowSystem::freeClosedWindows() {
		std::unique_lock lock(m_WindowThreadAccess);
		m_WindowThreads.erase(
			std::remove_if(
				m_WindowThreads.begin(),
				m_WindowThreads.end(),
				[this](const Scope<WindowThread>& wt) {
					return !wt || !wt->isRunning();
				}
			),
			m_WindowThreads.end()
		);
	}

	void WindowSystem::callEvent(Event& e) {
		std::shared_lock lock(m_WindowThreadAccess);
		for (Scope<WindowThread>& wt : m_WindowThreads) {
			wt->getWindow().callEvent(e);
		}
	}

	const std::vector<Ref<Monitor>>& WindowSystem::getMonitors() const {
		return m_Monitors;
	}

	Ref<Monitor> WindowSystem::getPrimaryMonitor() const {
		C78E_CORE_VALIDATE(m_PrimaryMonitorIndex != -1 && m_PrimaryMonitorIndex < m_Monitors.size(),
			return nullptr, "WindowSystem::getPrimaryMonitor: PrimaryMonitorIndex was out of bounds!");
		return m_Monitors[m_PrimaryMonitorIndex];
	}
}
