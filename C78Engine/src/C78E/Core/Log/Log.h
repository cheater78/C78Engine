#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/UUID.h>
#include <C78E/Utils/StdUtils.h>
#include <C78E/Utils/LogUtils.h>
#include <C78E/Math/Core/GLM.h>

namespace C78E {

	using LogCallbackFn = std::function<void(std::vector<std::string>)>;

	class Log {
	public:
		static void init();

		inline static Ref<spdlog::logger> getCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger> getClientLogger() { return s_ClientLogger; }
		inline static Ref<spdlog::sinks::ringbuffer_sink_mt> getRingBufferSink() { return s_RingbufferSink; }

	private:
		static const size_t c_MaxLogLines = 8192;

		static Scope<spdlog::formatter> s_Formatter;

		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

		static Ref<spdlog::sinks::ringbuffer_sink_mt> s_RingbufferSink;
		static Ref<spdlog::sinks::basic_file_sink_mt> s_FileLoggerSink;


		static std::vector<LogCallbackFn> s_logCallbackFns;
		static std::vector<std::string> s_lastLogLine;
	};

}

#define DefineFormatter(type, func)																		\
struct fmt::formatter<type> {																			\
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {							\
		return ctx.end();																				\
	}																									\
																										\
	template <typename FormatContext>																	\
	auto format(const type& input, FormatContext& ctx) const -> decltype(ctx.out()) {					\
		return fmt::format_to(ctx.out(), "{}", func);													\
	}																									\
}



template<>
DefineFormatter(std::filesystem::path, input.string());

template<>
DefineFormatter(std::stringstream, input.str());

template<>
DefineFormatter(C78E::UUID, C78E::UUID::encodeToString(input));

template<glm::length_t L, typename T, glm::qualifier Q>
DefineFormatter(C78E_EXPANDALL_MACRO(glm::vec<L, T, Q>), glm::to_string(input));

template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
DefineFormatter(C78E_EXPANDALL_MACRO(glm::mat<C, R, T, Q>), glm::to_string(input));

template<typename T, glm::qualifier Q>
DefineFormatter(C78E_EXPANDALL_MACRO(glm::qua<T, Q>), glm::to_string(input));


#ifdef C78E_DEBUG

#define C78E_CORE_FATAL(...)     { ::C78E::Log::getCoreLogger()->critical(__VA_ARGS__);	}
#define C78E_CORE_ERROR(...)     { ::C78E::Log::getCoreLogger()->error(__VA_ARGS__);		}
#define C78E_CORE_WARN(...)      { ::C78E::Log::getCoreLogger()->warn(__VA_ARGS__);		}
#define C78E_CORE_INFO(...)      { ::C78E::Log::getCoreLogger()->info(__VA_ARGS__);		}
#define C78E_CORE_TRACE(...)     { ::C78E::Log::getCoreLogger()->trace(__VA_ARGS__);		}

#define C78E_FATAL(...)          { ::C78E::Log::getClientLogger()->critical(__VA_ARGS__);}
#define C78E_ERROR(...)          { ::C78E::Log::getClientLogger()->error(__VA_ARGS__);	}
#define C78E_WARN(...)           { ::C78E::Log::getClientLogger()->warn(__VA_ARGS__);	}
#define C78E_INFO(...)           { ::C78E::Log::getClientLogger()->info(__VA_ARGS__);	}
#define C78E_TRACE(...)          { ::C78E::Log::getClientLogger()->trace(__VA_ARGS__);	}

#elif C78E_RELEASE																		

#define C78E_CORE_FATAL(...)     { ::C78E::Log::getCoreLogger()->critical(__VA_ARGS__);	}
#define C78E_CORE_ERROR(...)     { ::C78E::Log::getCoreLogger()->error(__VA_ARGS__);		}
#define C78E_CORE_WARN(...)      { ::C78E::Log::getCoreLogger()->warn(__VA_ARGS__);		}
#define C78E_CORE_INFO(...)      { ::C78E::Log::getCoreLogger()->info(__VA_ARGS__);		}
#define C78E_CORE_TRACE(...)     { ::C78E::Log::getCoreLogger()->trace(__VA_ARGS__);		}

#define C78E_FATAL(...)          { ::C78E::Log::getClientLogger()->critical(__VA_ARGS__);}
#define C78E_ERROR(...)          { ::C78E::Log::getClientLogger()->error(__VA_ARGS__);	}
#define C78E_WARN(...)           { ::C78E::Log::getClientLogger()->warn(__VA_ARGS__);	}
#define C78E_INFO(...)           { ::C78E::Log::getClientLogger()->info(__VA_ARGS__);	}
#define C78E_TRACE(...)          { ::C78E::Log::getClientLogger()->trace(__VA_ARGS__);	}

#elif C78E_DIST																			
				
#define C78E_CORE_FATAL(...)		{														}
#define C78E_CORE_ERROR(...)		{														}
#define C78E_CORE_WARN(...)		{														}
#define C78E_CORE_INFO(...)		{														}
#define C78E_CORE_TRACE(...)		{														}

#define C78E_FATAL(...)			{														}
#define C78E_ERROR(...)			{														}
#define C78E_WARN(...)			{														}
#define C78E_INFO(...)			{														}
#define C78E_TRACE(...)			{														}

#else

#define C78E_CORE_FATAL(...)		{														}
#define C78E_CORE_ERROR(...)		{														}
#define C78E_CORE_WARN(...)		{														}
#define C78E_CORE_INFO(...)		{														}
#define C78E_CORE_TRACE(...)		{														}

#define C78E_FATAL(...)			{														}
#define C78E_ERROR(...)			{														}
#define C78E_WARN(...)			{														}
#define C78E_INFO(...)			{														}
#define C78E_TRACE(...)			{														}

#endif
