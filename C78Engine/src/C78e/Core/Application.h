#pragma once

#include "C78E/Core/Core.h"
#include "C78E/Core/Timer.h"
#include "C78E/Core/Window.h"
#include "C78E/Core/LayerStack.h"
#include <C78E/ImGui/ImGuiLayer.h>
#include "C78E/Core/Console.h"
#include "C78E/Events/Event.h"
#include "C78E/Events/ApplicationEvent.h"


namespace C78E {

	class C78_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		void pushLayer(Ref<Layer> layer);
		void pushOverlay(Ref<Layer> layer);

		Window& getWindow() { return *m_Window.get(); }

		static Application& get() { return *s_App; }

	private:
		//Singelton Application
		static Application* s_App;

		Scope<Window> m_Window;
		bool m_Running = true;

		Scope<Timer> m_RunTime;
		LayerStack m_LayerStack;
		Ref<ImGuiLayer> m_ImGuiLayer;
		Ref<Console> m_Console;


		//Events
		bool onWindowClose(WindowCloseEvent e);
		bool onWindowResize(WindowResizeEvent e);


		//Commands
		void onCMDClose(std::string cmd);
	};
	
	Application* createApplication();

}

