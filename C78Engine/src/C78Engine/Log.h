#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace C78Engine {

	class C78E_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }


	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#ifdef BUILD_DIST
	//CORE
	#define C78E_CORE_FATAL(...)	
	#define C78E_CORE_ERROR(...)	
	#define C78E_CORE_WARN(...)		
	#define C78E_CORE_INFO(...)		
	#define C78E_CORE_TRACE(...)	

	//CLIENT
	#define C78E_FATAL(...)			
	#define C78E_ERROR(...)			
	#define C78E_WARN(...)			
	#define C78E_INFO(...)			
	#define C78E_TRACE(...)			
#else
	//CORE
	#define C78E_CORE_FATAL(...)	::C78Engine::Log::GetCoreLogger()->fatal(__VA_ARGS__);
	#define C78E_CORE_ERROR(...)	::C78Engine::Log::GetCoreLogger()->error(__VA_ARGS__);
	#define C78E_CORE_WARN(...)		::C78Engine::Log::GetCoreLogger()->warn(__VA_ARGS__);
	#define C78E_CORE_INFO(...)		::C78Engine::Log::GetCoreLogger()->info(__VA_ARGS__);
	#define C78E_CORE_TRACE(...)	::C78Engine::Log::GetCoreLogger()->trace(__VA_ARGS__);

	//CLIENT
	#define C78E_FATAL(...)			::C78Engine::Log::GetClientLogger()->fatal(__VA_ARGS__);
	#define C78E_ERROR(...)			::C78Engine::Log::GetClientLogger()->error(__VA_ARGS__);
	#define C78E_WARN(...)			::C78Engine::Log::GetClientLogger()->warn(__VA_ARGS__);
	#define C78E_INFO(...)			::C78Engine::Log::GetClientLogger()->info(__VA_ARGS__);
	#define C78E_TRACE(...)			::C78Engine::Log::GetClientLogger()->trace(__VA_ARGS__);
#endif // BUILD_DIST



