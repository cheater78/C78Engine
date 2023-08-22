#pragma once
#pragma once

#ifdef C78_PLATFORM_WINDOWS
	#ifdef C78_BUILD_DLL
		#define C78_API __declspec(dllexport)
	#else
		#define C78_API __declspec(dllimport)
	#endif // C78_BUILD_DLL
#else
	#error WinOnlyForNow!
#endif // C78_PLATFORM_WINDOWS

#ifdef C78_ENABLE_ASSERTS
	#include "C78e/Core/Log.h"
	#define C78_CORE_ASSERT(x, ...) { if(!x) { C78_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define C78_ASSERT(x, ...) { if(!x) { C78_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define C78_CORE_ASSERT(x, ...)
	#define C78_ASSERT(x, ...)
#endif // C78_ENABLE_ASSERTS

#define BIT(x) (1<<x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)