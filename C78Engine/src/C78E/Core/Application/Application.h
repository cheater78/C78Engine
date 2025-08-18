#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Timer.h>
#include <C78E/Core/Window/Window.h>
#include <C78E/Core/Events/Event.h>

namespace C78E {

	/**
	 * @brief The main Application class, that manages the application lifecycle, windows, and events.
	 * This class is responsible for initializing the application, creating windows, handling events,
	 * and running the main loop. Singleton, there can be multiple windows, but only one Application instance.
	 */
	class Application {
	public:
		static bool exists() { return s_App; }
		static Application& get() { return *s_App; }
	public: // Application Interface
		Application();
		virtual ~Application();

		// Application Info -> no changing at Runtime, override for own custom app info
		virtual const char* getApplicationName() const { return C78E_APP_DEFAULT_NAME; }
		virtual uint32_t getApplicationVersion() const { return C78E_APP_DEFAULT_VERSION_NUMBER; }

		// Optional Event handling method, can be overridden by derived classes
		virtual void onEvent(Event& e) {}

	public:
		void run(); // Main loop, runs the application until it is closed
		void close(); // Close the application
	public:
		Window& createWindow(const WindowProperties& props = {});
	private:
		void applicationEventCallbackFunction(Event& e);
	private:
		bool m_Running = true;
		Scope<Timer> m_RunTime;
		std::vector<Scope<Window>> m_Windows; // desktop windows
		std::vector<Scope<Window>> m_WindowCreateQueue; // windows to be created

	private:
		static Application* s_App; //Application Singelton
	};
	
	Application* createApplication();

}