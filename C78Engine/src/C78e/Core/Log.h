#pragma once
#include "C78e/Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <spdlog/sinks/ringbuffer_sink.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#pragma warning(pop)
//----


namespace std {
	_EXPORT_STD _NODISCARD inline string to_string(const char* _Val) {
		std::string ret = _Val;
		return ret;
	}
	_EXPORT_STD _NODISCARD inline string to_string(string _Val) {
		return _Val;
	}
}

namespace C78e {
	using LogCallbackFn = std::function<void(std::vector<std::string>)>;
	class C78_API Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger;  }
		inline static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt>& getRingBufferSink() { return s_RingbufferSink; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

		static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_RingbufferSink;
		static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> s_FileLoggerSink;


		static std::vector<LogCallbackFn> s_logCallbackFns;
		static std::vector<std::string> s_lastLogLine;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

#ifdef C78_DEBUG

#define C78_CORE_FATAL(...)     { ::C78e::Log::getCoreLogger()->critical(__VA_ARGS__);	}
#define C78_CORE_ERROR(...)     { ::C78e::Log::getCoreLogger()->error(__VA_ARGS__);		}
#define C78_CORE_WARN(...)      { ::C78e::Log::getCoreLogger()->warn(__VA_ARGS__);		}
#define C78_CORE_INFO(...)      { ::C78e::Log::getCoreLogger()->info(__VA_ARGS__);		}
#define C78_CORE_TRACE(...)     { ::C78e::Log::getCoreLogger()->trace(__VA_ARGS__);		}

#define C78_FATAL(...)          { ::C78e::Log::getClientLogger()->critical(__VA_ARGS__);}
#define C78_ERROR(...)          { ::C78e::Log::getClientLogger()->error(__VA_ARGS__);	}
#define C78_WARN(...)           { ::C78e::Log::getClientLogger()->warn(__VA_ARGS__);	}
#define C78_INFO(...)           { ::C78e::Log::getClientLogger()->info(__VA_ARGS__);	}
#define C78_TRACE(...)          { ::C78e::Log::getClientLogger()->trace(__VA_ARGS__);	}

#elif C78_RELEASE																		

#define C78_CORE_FATAL(...)     { ::C78e::Log::getCoreLogger()->critical(__VA_ARGS__);	}
#define C78_CORE_ERROR(...)     { ::C78e::Log::getCoreLogger()->error(__VA_ARGS__);		}
#define C78_CORE_WARN(...)      { ::C78e::Log::getCoreLogger()->warn(__VA_ARGS__);		}
#define C78_CORE_INFO(...)      { ::C78e::Log::getCoreLogger()->info(__VA_ARGS__);		}
#define C78_CORE_TRACE(...)     { ::C78e::Log::getCoreLogger()->trace(__VA_ARGS__);		}

#define C78_FATAL(...)          { ::C78e::Log::getClientLogger()->critical(__VA_ARGS__);}
#define C78_ERROR(...)          { ::C78e::Log::getClientLogger()->error(__VA_ARGS__);	}
#define C78_WARN(...)           { ::C78e::Log::getClientLogger()->warn(__VA_ARGS__);	}
#define C78_INFO(...)           { ::C78e::Log::getClientLogger()->info(__VA_ARGS__);	}
#define C78_TRACE(...)          { ::C78e::Log::getClientLogger()->trace(__VA_ARGS__);	}

#elif C78_DIST																			
				
#define C78_CORE_FATAL(...)		{														}
#define C78_CORE_ERROR(...)		{														}
#define C78_CORE_WARN(...)		{														}
#define C78_CORE_INFO(...)		{														}
#define C78_CORE_TRACE(...)		{														}

#define C78_FATAL(...)			{														}
#define C78_ERROR(...)			{														}
#define C78_WARN(...)			{														}
#define C78_INFO(...)			{														}
#define C78_TRACE(...)			{														}

#endif
