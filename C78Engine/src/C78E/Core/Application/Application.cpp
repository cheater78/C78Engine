#include "C78EPCH.h"
#include "Application.h"

#include <C78E/Core/Events/WindowEvent.h>

namespace C78E {

	Application* Application::s_App = nullptr;

	Application::Application() {
		C78E_CORE_ASSERT(!s_App, "There can only be one Application!");
		s_App = this;
		m_RunTime = createScope<Timer>();
	}

	Application::~Application() {
		/*
		* Windows and their Layers are automatically destructed
		*/
	}

	void Application::run() {
		while (m_Running) {
			Timestep delta = m_RunTime->elapsedSeconds(); //TODO: give Window their own Thread and therefore runtime
			m_RunTime->reset();

			for(Scope<Window>& window : m_Windows) {
				if (window->isRunning()) {
					window->callUpdate(delta);
				}
			}

			// Purge closed windows - O(n)
			m_Windows.erase(
				std::remove_if(
					m_Windows.begin(),
					m_Windows.end(),
					[](const Scope<Window>& window) {
						return !window->isRunning();
					}
				),
				m_Windows.end()
			);

			// Move newly created windows from the queue to the main window list
			m_WindowCreateQueue.erase(
				std::remove_if(
					m_WindowCreateQueue.begin(),
					m_WindowCreateQueue.end(),
					[this](Scope<Window>& window) {
						if (window->isRunning()) {
							m_Windows.emplace_back(std::move(window));
						}
						return true; // remove from queue
					}
				),
				m_WindowCreateQueue.end()
			);

			if(m_Windows.empty()) {
				m_Running = false; // No windows left, close the application
			}

		}
	}

	void Application::close() {
		for(Scope<Window>& window : m_Windows) {
			window->close();
		}
		m_Running = false;
	}

	Window& Application::createWindow(const WindowProperties& props) {
		return *m_WindowCreateQueue.emplace_back(Window::create(props, C78E_BIND_THIS_METHOD(Application::applicationEventCallbackFunction)));
	}

	void Application::applicationEventCallbackFunction(Event& e) {
		EventDispatcher dispatcher(e);
		// Dispatch window events to the window itself
		dispatcher.dispatch<WindowEvent>(
			[](WindowEvent& event) -> void {
				event.getWindow().callEvent(event);
				// dont share for now
				event.handled = true;
			}
		);

		if (e.handled) {
			return;
		}

		for (auto it = m_Windows.begin(); it != m_Windows.end(); it++) {
			if (e.handled) {
				break;
			}
			(*it)->callEvent(e);
		}
	}

	
}


