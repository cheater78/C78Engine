#pragma once

#ifdef C78E_PLATFORM_WINDOWS
	#define C78E_DEBUGBREAK() __debugbreak()
// C78E_PLATFORM_WINDOWS
#elif C78E_PLATFORM_LINUX
	#include <stdio.h>
	#include <signal.h>
	#define C78E_DEBUGBREAK() raise(SIGTRAP)
// C78E_PLATFORM_LINUX
#else
	#error NotSupportedPlatform!
#endif // C78E_PLATFORM_*


// Bit Map
#define BIT(x) (1<<x)

#define BIND_CALLBACK_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define C78E_EXPAND_MACRO(x) x
#define C78E_EXPANDALL_MACRO(...) __VA_ARGS__
#define C78E_STRINGIFY_MACRO(x) #x

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

	template<typename T>
	using WRef = std::weak_ptr<T>;
	template<typename T, typename ... Args>
	constexpr WRef<T> createWRef(Args&& ... args) {
		return std::weak_ptr<T>(std::forward<Args>(args)...);
	}

}
