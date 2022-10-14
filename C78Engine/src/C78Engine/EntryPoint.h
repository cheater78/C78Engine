#pragma once

#ifdef C78E_PLATFORM_WINDOWS
#include "Application.h"
#include "Log.h"

extern C78Engine::Application* C78Engine::CreateApplication();



int main(signed int argc, char** argv)
{

	C78Engine::Log::Init();
	C78E_CORE_INFO("Starting LOG...");
	
	auto app = C78Engine::CreateApplication();
	app->Run();

	delete app;
}

#endif