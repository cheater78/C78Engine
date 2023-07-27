#pragma once

#include "Application.h"

#ifdef C78_PLATFORM_WINDOWS

extern C78e::Application* C78e::createApplication();

int main(int argc, char** argv)
{
	auto app = C78e::createApplication();
	app->run();
	delete app;
	return 0;
}

#endif // C78_PLATFORM_WINDOWS
