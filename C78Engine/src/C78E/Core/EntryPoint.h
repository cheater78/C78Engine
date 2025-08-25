#pragma once
#include <C78E/Core/Application/Application.h>
//#include <C78E/Renderer/API/GraphicsInstance.h>

extern C78E::Application* C78E::createApplication();

//TODO: Nicer command line argument handling - allow application to specify its own arguments
//void parseCommandLineArguments(int argc, char** argv) {
//	for(int i = 0; i < argc; i++) {
//		std::string arg = argv[i];
//		C78E_CORE_TRACE("Command Line Argument: {}", arg);
//
//		if(arg == "--help" || arg == "-h") {
//			C78E_CORE_INFO("Usage: C78Engine [options]");
//			C78E_CORE_INFO("Options:");
//			C78E_CORE_INFO("  --help, -h				Show this help message");
//			C78E_CORE_INFO("  --version, -v				Show version information");
//			C78E_CORE_INFO("  --api=[OpenGL,Vulkan],    Specify the RenderAPI to use");
//			exit(0);
//		} else if(arg == "--version" || arg == "-v") {
//			C78E_CORE_INFO("C78Engine Version {}", C78E_APP_DEFAULT_VERSION_STRING);
//			exit(0);
//		} else if(arg.starts_with("--api=")) {
//			std::string api = arg.substr(arg.find('=') + 1);
//			if(api == "OpenGL") {
//				C78E_CORE_INFO("Using OpenGL RenderAPI");
//				C78E::GraphicsInstance::create(C78E::API::OpenGL);
//			} else if(api == "Vulkan") {
//				C78E_CORE_INFO("Using Vulkan RenderAPI");
//				C78E::GraphicsInstance::create(C78E::API::Vulkan);
//			} else {
//				C78E_CORE_ERROR("Unknown RenderAPI: {}", api);
//				exit(1);
//			}
//		} else {
//			C78E_CORE_WARN("Unknown command line argument: {}", arg);
//		}
//	}
//}

int main(int argc, char** argv) {
	C78E::Log::init();
	C78E_CORE_INFO("Logger started...");
	C78E_INFO("Logger started...");

	auto app = C78E::createApplication();
	app->run();
	delete app;
	return 0;
}

