#pragma once

#include "Core.h"

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

