#pragma once
#include "C78e/Core/Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace C78e {
	class C78_API Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger;  }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#ifdef C78_DEBUG

#define C78_CORE_FATAL(...)      ::C78e::Log::getCoreLogger()->critical(__VA_ARGS__)
#define C78_CORE_ERROR(...)      ::C78e::Log::getCoreLogger()->error(__VA_ARGS__)
#define C78_CORE_WARN(...)       ::C78e::Log::getCoreLogger()->warn(__VA_ARGS__)
#define C78_CORE_INFO(...)       ::C78e::Log::getCoreLogger()->info(__VA_ARGS__)
#define C78_CORE_TRACE(...)      ::C78e::Log::getCoreLogger()->trace(__VA_ARGS__)

#define C78_FATAL(...)           ::C78e::Log::getClientLogger()->critical(__VA_ARGS__)
#define C78_ERROR(...)           ::C78e::Log::getClientLogger()->error(__VA_ARGS__)
#define C78_WARN(...)            ::C78e::Log::getClientLogger()->warn(__VA_ARGS__)
#define C78_INFO(...)            ::C78e::Log::getClientLogger()->info(__VA_ARGS__)
#define C78_TRACE(...)           ::C78e::Log::getClientLogger()->trace(__VA_ARGS__)

#elif C78_RELEASE

#define C78_CORE_FATAL(...)      ::C78e::Log::getCoreLogger()->critical(__VA_ARGS__)
#define C78_CORE_ERROR(...)      ::C78e::Log::getCoreLogger()->error(__VA_ARGS__)
#define C78_CORE_WARN(...)       ::C78e::Log::getCoreLogger()->warn(__VA_ARGS__)
#define C78_CORE_INFO(...)       ::C78e::Log::getCoreLogger()->info(__VA_ARGS__)
#define C78_CORE_TRACE(...)      ::C78e::Log::getCoreLogger()->trace(__VA_ARGS__)

#define C78_FATAL(...)           ::C78e::Log::getClientLogger()->critical(__VA_ARGS__)
#define C78_ERROR(...)           ::C78e::Log::getClientLogger()->error(__VA_ARGS__)
#define C78_WARN(...)            ::C78e::Log::getClientLogger()->warn(__VA_ARGS__)
#define C78_INFO(...)            ::C78e::Log::getClientLogger()->info(__VA_ARGS__)
#define C78_TRACE(...)           ::C78e::Log::getClientLogger()->trace(__VA_ARGS__)

#elif C78_DIST

#define C78_CORE_FATAL(...)
#define C78_CORE_ERROR(...)
#define C78_CORE_WARN(...)
#define C78_CORE_INFO(...)
#define C78_CORE_TRACE(...)

#define C78_FATAL(...)
#define C78_ERROR(...)
#define C78_WARN(...)
#define C78_INFO(...)
#define C78_TRACE(...)

#endif
