#pragma once

#include "C78e/Core/Core.h"
#include "Window.h"
#include "C78e/Events/Event.h"
#include "C78e/Events/ApplicationEvent.h"
#include "C78e/Core/LayerStack.h"
#include <C78e/ImGui/ImGuiLayer.h>
#include "C78e/Console/Console.h"

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
		Console* m_Console;

		bool onWindowClose(WindowCloseEvent e);
		bool onCMDClose(std::string cmd);

		void closeWindow();
		
	};
	
	Application* createApplication();

}

