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
		m_Window->setEventCallback(BIND_CALLBACK_FN(Application::onEvent));

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

		m_Console = new Console("C78e Console");
		pushOverlay(m_Console);

		m_Console->addCmd("stop", BIND_CALLBACK_FN(Application::onCMDClose));

	}

	Application::~Application() {	
	}

	void Application::run()
	{
		std::srand(7539475345);
		while (m_Running) {
			glClearColor(0.1f, 0.1f, 0.15f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->onUpdate();


			m_ImGuiLayer->begin();

			for (Layer* layer : m_LayerStack) {
				layer->onImGuiRender();
			}
				

			m_ImGuiLayer->end();

			m_Window->onUpdate();
		}
		
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_CALLBACK_FN(Application::onWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::onWindowClose(WindowCloseEvent e) {
		closeWindow();
		return true;
	}
	bool Application::onCMDClose(std::string cmd) {
		closeWindow();
		return true;
	}

	void Application::closeWindow()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		m_Running = false;
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


