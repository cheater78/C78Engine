#pragma once

#include "C78e/Application.h"

#ifdef C78_PLATFORM_WINDOWS

extern C78e::Application* C78e::createApplication();

int main(int argc, char** argv)
{
	C78e::Log::init();
	C78_CORE_INFO("Logger started...");
	C78_INFO("Logger started...");

	auto app = C78e::createApplication();
	app->run();
	delete app;
	return 0;
}

#elif C78_PLATFORM_LINUX

extern C78e::Application* C78e::createApplication();

int main(int argc, char** argv)
{
	C78e::Log::init();
	C78_CORE_INFO("Logger started...");
	C78_INFO("Logger started...");

	auto app = C78e::createApplication();
	app->run();
	delete app;
	return 0;
}

#endif // C78_PLATFORM_WINDOWS
