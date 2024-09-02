#pragma once
#include "C78E/Core/Core.h"
#include "C78E/Utils/StdUtils.h"
#include "C78E/Core/UUID.h"

#include <filesystem>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#pragma warning(pop)
//----

namespace C78E {

	using LogCallbackFn = std::function<void(std::vector<std::string>)>;

	class Log {
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger> getCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> getEditorLogger() { return s_EditorLogger; }
		inline static std::shared_ptr<spdlog::logger> getClientLogger() { return s_ClientLogger; }
		inline static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> getRingBufferSink() { return s_RingbufferSink; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_EditorLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

		static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_RingbufferSink;
		static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> s_FileLoggerSink;


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
	auto format(const type& input, FormatContext& ctx) -> decltype(ctx.out()) {							\
		return fmt::format_to(ctx.out(),																		\
			"{}",																						\
			func);																						\
	}																									\
}



template<>
DefineFormatter(std::filesystem::path, input.string());

template<>
DefineFormatter(std::stringstream, input.str());

template<>
DefineFormatter(C78E::UUID, C78E::UUID::toString(input));

template<glm::length_t L, typename T, glm::qualifier Q>
DefineFormatter(C78_EXPANDALL_MACRO(glm::vec<L, T, Q>), glm::to_string(input));

template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
DefineFormatter(C78_EXPANDALL_MACRO(glm::mat<C, R, T, Q>), glm::to_string(input));

template<typename T, glm::qualifier Q>
DefineFormatter(C78_EXPANDALL_MACRO(glm::qua<T, Q>), glm::to_string(input));


#ifdef C78_DEBUG

#define C78_CORE_FATAL(...)     { ::C78E::Log::getCoreLogger()->critical(__VA_ARGS__);	}
#define C78_CORE_ERROR(...)     { ::C78E::Log::getCoreLogger()->error(__VA_ARGS__);		}
#define C78_CORE_WARN(...)      { ::C78E::Log::getCoreLogger()->warn(__VA_ARGS__);		}
#define C78_CORE_INFO(...)      { ::C78E::Log::getCoreLogger()->info(__VA_ARGS__);		}
#define C78_CORE_TRACE(...)     { ::C78E::Log::getCoreLogger()->trace(__VA_ARGS__);		}

#define C78_EDITOR_FATAL(...)	{ ::C78E::Log::getEditorLogger()->critical(__VA_ARGS__);	}
#define C78_EDITOR_ERROR(...)	{ ::C78E::Log::getEditorLogger()->error(__VA_ARGS__);		}
#define C78_EDITOR_WARN(...)	{ ::C78E::Log::getEditorLogger()->warn(__VA_ARGS__);		}
#define C78_EDITOR_INFO(...)	{ ::C78E::Log::getEditorLogger()->info(__VA_ARGS__);		}
#define C78_EDITOR_TRACE(...)	{ ::C78E::Log::getEditorLogger()->trace(__VA_ARGS__);		}

#define C78_FATAL(...)          { ::C78E::Log::getClientLogger()->critical(__VA_ARGS__);}
#define C78_ERROR(...)          { ::C78E::Log::getClientLogger()->error(__VA_ARGS__);	}
#define C78_WARN(...)           { ::C78E::Log::getClientLogger()->warn(__VA_ARGS__);	}
#define C78_INFO(...)           { ::C78E::Log::getClientLogger()->info(__VA_ARGS__);	}
#define C78_TRACE(...)          { ::C78E::Log::getClientLogger()->trace(__VA_ARGS__);	}

#elif C78_RELEASE																		

#define C78_CORE_FATAL(...)     { ::C78E::Log::getCoreLogger()->critical(__VA_ARGS__);	}
#define C78_CORE_ERROR(...)     { ::C78E::Log::getCoreLogger()->error(__VA_ARGS__);		}
#define C78_CORE_WARN(...)      { ::C78E::Log::getCoreLogger()->warn(__VA_ARGS__);		}
#define C78_CORE_INFO(...)      { ::C78E::Log::getCoreLogger()->info(__VA_ARGS__);		}
#define C78_CORE_TRACE(...)     { ::C78E::Log::getCoreLogger()->trace(__VA_ARGS__);		}

#define C78_EDITOR_FATAL(...)	{ ::C78E::Log::getEditorLogger()->critical(__VA_ARGS__);	}
#define C78_EDITOR_ERROR(...)	{ ::C78E::Log::getEditorLogger()->error(__VA_ARGS__);		}
#define C78_EDITOR_WARN(...)	{ ::C78E::Log::getEditorLogger()->warn(__VA_ARGS__);		}
#define C78_EDITOR_INFO(...)	{ ::C78E::Log::getEditorLogger()->info(__VA_ARGS__);		}
#define C78_EDITOR_TRACE(...)	{ ::C78E::Log::getEditorLogger()->trace(__VA_ARGS__);		}

#define C78_FATAL(...)          { ::C78E::Log::getClientLogger()->critical(__VA_ARGS__);}
#define C78_ERROR(...)          { ::C78E::Log::getClientLogger()->error(__VA_ARGS__);	}
#define C78_WARN(...)           { ::C78E::Log::getClientLogger()->warn(__VA_ARGS__);	}
#define C78_INFO(...)           { ::C78E::Log::getClientLogger()->info(__VA_ARGS__);	}
#define C78_TRACE(...)          { ::C78E::Log::getClientLogger()->trace(__VA_ARGS__);	}

#elif C78_DIST																			
				
#define C78_CORE_FATAL(...)		{														}
#define C78_CORE_ERROR(...)		{														}
#define C78_CORE_WARN(...)		{														}
#define C78_CORE_INFO(...)		{														}
#define C78_CORE_TRACE(...)		{														}

#define C78_EDITOR_FATAL(...)	{														}
#define C78_EDITOR_ERROR(...)	{														}
#define C78_EDITOR_WARN(...)	{														}
#define C78_EDITOR_INFO(...)	{														}
#define C78_EDITOR_TRACE(...)	{														}

#define C78_FATAL(...)			{														}
#define C78_ERROR(...)			{														}
#define C78_WARN(...)			{														}
#define C78_INFO(...)			{														}
#define C78_TRACE(...)			{														}

#endif
