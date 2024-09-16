#include "C78EPCH.h"
#include "Application.h"

namespace C78E {

	Application* Application::s_App = nullptr;

	Application::Application(const WindowProps& windowProperties) {
		C78_CORE_ASSERT(!s_App, "There can only be one Application!");
		s_App = this;

		m_RunTime = createScope<Timer>();

		m_Console = createRef<Console>("C78E Console");
		m_Console->addCmd("exit", BIND_CALLBACK_FN(Application::onCMDClose));

		m_Window = Window::create(windowProperties);
		m_Window->setEventCallback(BIND_CALLBACK_FN(Application::onEvent));

		m_ImGuiLayer = createRef<GUI::ImGuiLayer>();

		pushOverlay(m_ImGuiLayer);
		pushOverlay(m_Console);
	}

	Application::~Application() { }

	void Application::run() {
		while (m_Running) {
			Timestep delta = m_RunTime->elapsedSeconds();
			m_RunTime->reset();

			for (Ref<Layer> layer : m_LayerStack)
				layer->onUpdate(delta);

			m_ImGuiLayer->begin();
			for (Ref<Layer> layer : m_LayerStack)
				layer->onImGuiRender();

			m_ImGuiLayer->end();

			m_Window->onUpdate();
		}
	}

	void Application::pushLayer(Ref<Layer> layer) {
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Ref<Layer> layer) {
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}


	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.Handled)
				break;
		}

		dispatcher.dispatch<WindowCloseEvent>(BIND_CALLBACK_FN(Application::onWindowCloseEvent));
	}
	bool Application::onWindowCloseEvent(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}


	void Application::onCMDClose(std::string cmd) { m_Running = false; }
}


