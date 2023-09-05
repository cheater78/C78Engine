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

	#define C78_DEBUGBREAK() __debugbreak()

#elif C78_PLATFORM_LINUX
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
	#define C78_DEBUGBREAK() raise(SIGTRAP)

#else
	#error NotSupportedPlatform!
#endif // C78_PLATFORM_WINDOWS


// Bit Map
#define BIT(x) (1<<x)

#define BIND_CALLBACK_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define C78_EXPAND_MACRO(x) x
#define C78_STRINGIFY_MACRO(x) #x

#include <memory>
namespace C78e {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> createScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "C78e/Core/Log.h"