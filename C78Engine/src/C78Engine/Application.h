#pragma once

#include "Core.h"

namespace C78Engine {

	class C78E_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//define in Client
	Application* CreateApplication();
}