#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Timer.h>
#include <C78E/Core/Application/Window.h>
#include <C78E/Core/Application/LayerStack.h>
#include <C78E/Core/Application/ImGuiLayer.h>
#include <C78E/Core/Console.h>
#include <C78E/Events/Event.h>
#include <C78E/Events/ApplicationEvent.h>


namespace C78E {

	class Application {
	private:
		static Application* s_App; //Application Singelton
	public:
		static Application& get() { return *s_App; }
	public:
		Application(const Window::WindowProps& windowProperties = Window::WindowProps());
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		void pushLayer(Ref<Layer> layer);
		void pushOverlay(Ref<Layer> layer);

		void close();

		Window& getWindow() { return *m_Window.get(); }
	protected:
		Ref<ImGuiLayer> getImGuiLayer() { return m_ImGuiLayer; }
	private:
		Scope<Window> m_Window; //multiple Windows?
		bool m_Running = true;

		Scope<Timer> m_RunTime;
		LayerStack m_LayerStack;
		Ref<ImGuiLayer> m_ImGuiLayer;
		Ref<Console> m_Console;

		bool onWindowCloseEvent(WindowCloseEvent& event);

		//Commands
		void onCMDClose(std::string cmd);
	};
	
	Application* createApplication();

}