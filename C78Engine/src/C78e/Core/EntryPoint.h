#pragma once
#include <C78E/Core/Application.h>

extern C78E::Application* C78E::createApplication();

int main(int argc, char** argv) {
	C78E::Log::init();
	C78_CORE_INFO("Logger started...");
	C78_INFO("Logger started...");

	auto app = C78E::createApplication();
	app->run();
	delete app;
	return 0;
}

