#pragma once

#include "C78E/Core/Core.h"
#include "C78E/Core/Timer.h"
#include "C78E/Core/Window.h"
#include "C78E/Core/LayerStack.h"
#include <C78E/GUI/ImGui/ImGuiLayer.h>
#include "C78E/Core/Console.h"
#include "C78E/Events/Event.h"
#include "C78E/Events/ApplicationEvent.h"


namespace C78E {

	class Application {
	private:
		static Application* s_App; //Application Singelton
	public:
		static Application& get() { return *s_App; }
	public:
		Application(const WindowProps& windowProperties = WindowProps());
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		void pushLayer(Ref<Layer> layer);
		void pushOverlay(Ref<Layer> layer);

		Window& getWindow() { return *m_Window.get(); }
	protected:
		Ref<GUI::ImGuiLayer> getImGuiLayer() { return m_ImGuiLayer; }
	private:
		Scope<Window> m_Window;
		bool m_Running = true;

		Scope<Timer> m_RunTime;
		LayerStack m_LayerStack;
		Ref<GUI::ImGuiLayer> m_ImGuiLayer;
		Ref<Console> m_Console;

		bool onWindowCloseEvent(WindowCloseEvent& event);

		//Commands
		void onCMDClose(std::string cmd);
	};
	
	Application* createApplication();

}

