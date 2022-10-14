#pragma once

#ifdef C78E_PLATFORM_WINDOWS
#include "Application.h"

extern C78Engine::Application* C78Engine::CreateApplication();



int main(signed int argc, char** argv)
{
	auto app = C78Engine::CreateApplication();
	app->Run();

	delete app;
}

#endif