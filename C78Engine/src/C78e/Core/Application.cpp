#include "C78ePCH.h"
#include "Application.h"

#include "Core.h"
#include "C78e/Events/ApplicationEvent.h"
#include "C78e/Core/Log.h"

#include <glad.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace C78e {

	Application* Application::s_App = nullptr;

	Application::Application()
	{
		C78_CORE_ASSERT(!s_App, "There can only be one Application!");
		s_App = this;

		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{
		for (Layer* layer : m_LayerStack)
			layer->onDetach();
	}

	void Application::run()
	{

		while (m_Running) {
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->onUpdate();


			m_ImGuiLayer->begin();

			for (Layer* layer : m_LayerStack) {
				C78_CORE_INFO(layer->getName());
				layer->onImGuiRender();
			}
				

			m_ImGuiLayer->end();

			m_Window->onUpdate();
		}
		
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::onWindowClose(WindowCloseEvent e) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		m_Running = false;
		return true;
	}

	void Application::pushLayer(Layer* layer) {
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer) {
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}

}


