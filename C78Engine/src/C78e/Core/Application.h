#pragma once

#include "C78e/Core/Core.h"
#include "Window.h"
#include "C78e/Events/Event.h"
#include "C78e/Events/ApplicationEvent.h"
#include "C78e/Core/LayerStack.h"
#include <C78e/ImGui/ImGuiLayer.h>

namespace C78e {

	class C78_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		Window& getWindow() { return *m_Window.get(); }

		static Application& get() { return *s_App; }

	private:
		//Singelton Application
		static Application* s_App;

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool onWindowClose(WindowCloseEvent e);

		
	};
	
	Application* createApplication();

}

