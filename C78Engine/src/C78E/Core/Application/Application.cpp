#include "C78EPCH.h"
#include "Application.h"

namespace C78E {

	Application* Application::s_App = nullptr;

	Application::Application() {
		C78E_CORE_ASSERT(!s_App, "There can only be one Application!");
		s_App = this;
		m_RunTime = createScope<Timer>();
		m_WindowSystem = WindowSystem::create();
	}

	Application::~Application() {
	}

	void Application::run() {
		while (m_Running) {

			//executeTasks();
			//m_WindowSystem->waitEvents(); //TODO: 
			//m_WindowSystem->callEvents();
			//
			//m_WindowSystem->freeClosedWindows(); 
			//m_Running = m_WindowSystem->hasWindows();
			
		}
	}

	void Application::close() {
		m_WindowSystem->closeAll();
		m_Running = false;
	}

	WindowSystem& Application::getWindowSystem() {
		return *m_WindowSystem;
	}

	void Application::applicationEventCallbackFunction(Event& e) {
		m_WindowSystem->callEvent(e);
	}

	
}


