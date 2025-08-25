#pragma once
#include <C78E/Core/Timer.h>
#include "Window.h"

namespace C78E {

	class WindowThread {
	public:
		WindowThread() = default;
		WindowThread(const WindowCreateInfo& createInfo);
		WindowThread(WindowThread&) = delete;
		WindowThread(WindowThread&&) = delete;
		~WindowThread();

		bool isRunning() const;

		Window& getWindow();

	private:
		void runner();
	private:
		bool m_Running = true;
		Scope<Window> m_Window = nullptr;
		Scope<Timer> m_RunTime = nullptr;
		std::thread m_Thread;
	};

}
