#pragma once

#ifdef C78_PLATFORM_WINDOWS
	#define C78_DEBUGBREAK() __debugbreak()
// C78_PLATFORM_WINDOWS
#elif C78_PLATFORM_LINUX
	#include <stdio.h>
	#include <signal.h>
	#define C78_DEBUGBREAK() raise(SIGTRAP)
// C78_PLATFORM_LINUX
#else
	#error NotSupportedPlatform!
#endif // C78_PLATFORM_*


// Bit Map
#define BIT(x) (1<<x)

#define BIND_CALLBACK_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define C78_EXPAND_MACRO(x) x
#define C78_STRINGIFY_MACRO(x) #x

#include <memory>
namespace C78E {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> createScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "C78E/Core/Log.h"
