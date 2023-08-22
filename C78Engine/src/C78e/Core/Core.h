#pragma once
#pragma once

#ifdef C78_PLATFORM_WINDOWS
#ifdef C78_DYN_LINK
	#ifdef C78_BUILD_DLL
		#define C78_API __declspec(dllexport)
	#else
		#define C78_API __declspec(dllimport)
	#endif // C78_BUILD_DLL
#else
	#define C78_API
#endif

#define DEBUG_BREAK __debugbreak()

#elif C78_PLATFORM_LINUX
    #ifdef C78_BUILD_DLL
		#define C78_API __attribute__((visibility("default")))
	#else
		#define C78_API
	#endif // C78_BUILD_DLL

#ifdef C78_DYN_LINK
	#ifdef C78_BUILD_DLL
		#define C78_API __attribute__((visibility("default")))
	#else
		#define C78_API __declspec(dllimport)
	#endif // C78_BUILD_DLL
#else
	#define C78_API
#endif
 
#include <stdio.h>
#include <signal.h>
#define DEBUG_BREAK raise(SIGTRAP)

#else
	#error NotSupportedPlatform!
#endif // C78_PLATFORM_WINDOWS

#ifdef C78_ENABLE_ASSERTS
	#include "C78e/Core/Log.h"
	#define C78_CORE_ASSERT(x, ...) { if(!x) { C78_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__);	DEBUG_BREAK; } }
	#define C78_ASSERT(x, ...)		{ if(!x) { C78_FATAL("Assertion Failed: {0}", __VA_ARGS__);			DEBUG_BREAK; } }
#else
	#define C78_CORE_ASSERT(x, ...)
	#define C78_ASSERT(x, ...)
#endif // C78_ENABLE_ASSERTS

#define BIT(x) (1<<x)

#define BIND_CALLBACK_FN(fn) std::bind(&fn, this, std::placeholders::_1)