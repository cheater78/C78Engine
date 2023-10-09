#pragma once

#include "C78E/Core/Core.h"
#include "C78E/Core/Log.h"
#include <filesystem>

#ifdef C78_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define C78_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { C78##type##ERROR(msg, __VA_ARGS__); C78_DEBUGBREAK(); } }
	#define C78_INTERNAL_ASSERT_WITH_MSG(type, check, ...) C78_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define C78_INTERNAL_ASSERT_NO_MSG(type, check) C78_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", C78_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define C78_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define C78_INTERNAL_ASSERT_GET_MACRO(...) C78_EXPAND_MACRO( C78_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, C78_INTERNAL_ASSERT_WITH_MSG, C78_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define C78_ASSERT(...) C78_EXPAND_MACRO( C78_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define C78_CORE_ASSERT(...) C78_EXPAND_MACRO( C78_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define C78_ASSERT(...)
	#define C78_CORE_ASSERT(...)
#endif
