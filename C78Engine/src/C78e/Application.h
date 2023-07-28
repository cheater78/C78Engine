#pragma once

#include "C78e/Core.h"
#include "Events/Event.h"

namespace C78e {

	class C78_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

	};
	
	Application* createApplication();

}

