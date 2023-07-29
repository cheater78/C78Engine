#include "C78ePCH.h"
#include "Application.h"

#include "C78e/Events/ApplicationEvent.h"
#include "C78e/Log.h"

#include "glad.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace C78e {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent));
	}

	Application::~Application()
	{
	}

	void Application::run()
	{

		while (m_Running) {
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->onUpdate();

			m_Window->onUpdate();
		}
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

		C78_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::onWindowClose(WindowCloseEvent e) {
		m_Running = false;
		return true;
	}

	void Application::pushLayer(Layer* layer) {
		m_LayerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer) {
		m_LayerStack.pushOverlay(layer);
	}

}


