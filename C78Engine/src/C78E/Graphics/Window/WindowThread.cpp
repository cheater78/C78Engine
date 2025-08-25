#include "C78EPCH.h"
#include "WindowThread.h"

#include <C78E/Core/Application/Application.h>

namespace C78E {

	WindowThread::WindowThread(const WindowCreateInfo& createInfo) {
		m_Window = Window::create(createInfo);
		m_RunTime = createScope<Timer>();
		m_Running = true;
		m_Thread = std::thread(C78E_BIND_THIS_METHOD_AS_IS(WindowThread::runner));
		C78E_CORE_TRACE("WindowThread::WindowThread: Created WindowThread({})", m_Window->getTitle());
	}

	WindowThread::~WindowThread() {
		C78E_CORE_TRACE("WindowThread::WindowThread: Closing WindowThread({})...", m_Window->getTitle());
		if (m_Thread.joinable()) {
			m_Thread.join();
		}
		C78E_CORE_TRACE("WindowThread::WindowThread: Closed WindowThread({})", m_Window->getTitle());
	}

	bool WindowThread::isRunning() const {
		return m_Running;
	}

	Window& WindowThread::getWindow() {
		return *m_Window;
	}

	void WindowThread::runner() {
		while (m_Window->isRunning()) {
			Timestep delta = m_RunTime->elapsed();
			m_RunTime->reset();

			m_Window->callUpdate(delta);

		}
		m_Running = false;
		C78E_CORE_TRACE("WindowThread::runner: WindowThreadRunner stopped({})", m_Window->getTitle());
	}

}
