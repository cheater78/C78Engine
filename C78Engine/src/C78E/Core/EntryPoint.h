#pragma once
#include <C78E/Core/Application/Application.h>

extern C78E::Application* C78E::createApplication();

int main(int argc, char** argv) {
	C78E::Log::init();
	C78E_CORE_INFO("Logger started...");
	C78E_INFO("Logger started...");

	auto app = C78E::createApplication();
	app->run();
	delete app;
	return 0;
}

