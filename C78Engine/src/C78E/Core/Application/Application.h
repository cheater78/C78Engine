#pragma once
#include <C78E/Core/Base.h>
#include <C78E/Core/Core.h>
#include <C78E/Core/Timer.h>
#include <C78E/Events/Event.h>
#include <C78E/Core/Thread.h>
#include <C78E/Graphics/Window/WindowSystem.h>

namespace C78E {

	/**
	 * @brief Application Singleton as main instance
	 */
	class Application : public CallableThread {
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
		WindowSystem& getWindowSystem();
	private:
		void applicationEventCallbackFunction(Event& e);
	private:
		bool m_Running = true;
		Scope<Timer> m_RunTime = nullptr;
		Scope<WindowSystem> m_WindowSystem = nullptr;
		
	private:
		static Application* s_App; //Application Singelton
	};
	
	using MainThread = Application;

	Application* createApplication();

}
