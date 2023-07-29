#pragma once

#include "C78e/Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "C78e/LayerStack.h"

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

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool onWindowClose(WindowCloseEvent e);


	};
	
	Application* createApplication();

}

