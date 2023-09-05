#include "C78ePCH.h"
#include "Log.h"

namespace C78e {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> Log::s_RingbufferSink;
	std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Log::s_FileLoggerSink;

	std::vector<LogCallbackFn> Log::s_logCallbackFns;
	std::vector<std::string> Log::s_lastLogLine;

	void Log::init(){

		s_RingbufferSink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(1024);
		s_RingbufferSink->set_pattern("%^[%T] %n: %v%$");
		s_FileLoggerSink = std::make_shared <spdlog::sinks::basic_file_sink_mt>("C78Engine.log");
		s_FileLoggerSink->set_pattern("%^[%T] %n: %v%$");

		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(s_FileLoggerSink);
		sinks.push_back(s_RingbufferSink);
		auto coreSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
		coreSink->set_pattern("%^[%T] %n: %v%$");
		sinks.push_back(coreSink);

		s_CoreLogger = std::make_shared<spdlog::logger>("C78e", sinks.begin(), sinks.end());
		s_CoreLogger->set_level(spdlog::level::trace);

		sinks.pop_back();
		auto appSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
		appSink->set_pattern("%^[%T] %n: %v%$");
		sinks.push_back(appSink);


		s_ClientLogger = std::make_shared<spdlog::logger>("APP", sinks.begin(), sinks.end());
		s_ClientLogger->set_level(spdlog::level::trace);
	}


}
