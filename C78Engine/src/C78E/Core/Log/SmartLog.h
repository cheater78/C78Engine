#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Log/Log.h>
#include <filesystem>

/*
* C78E_INTERNAL_SMARTLOG_IMPL
* contitional Log with variable escape strategy, don't use indexed formatter args!
* type -> _CORE_ or _
* check -> condition statement
* handling -> how a failed condition should be handled, e.g.: C78E_DEBUGBREAK(), return <something>, <do nothing>, ...
* premsg -> kind of smartLog, NO FORMATTER ARGS, e.g.: "Assertion", "Validation"
* msg -> (optional) message as formatter string literal
* ... -> formatter args for msg
*/
#define C78E_INTERNAL_SMARTLOG_IMPL(type, check, handling, premsg, msg, ...)																																	\
	if(!(check)) {																																															\
		C78E##type##ERROR(premsg " failed(File: {}, Line: {}, Condition: {}): " msg, std::filesystem::path(__FILE__).filename().string(), __LINE__, C78E_STRINGIFY_MACRO(check) __VA_OPT__(, __VA_ARGS__));	\
		handling;																																															\
	}

#ifdef C78E_ENABLE_ASSERTS
	#define C78E_ASSERT(check, ...)						C78E_EXPAND_MACRO(C78E_EXPAND_MACRO(C78E_INTERNAL_SMARTLOG_IMPL(_, check, C78E_DEBUGBREAK(), "Assertion", "" __VA_OPT__(__VA_ARGS__))))
	#define C78E_CORE_ASSERT(check, ...)					C78E_EXPAND_MACRO(C78E_EXPAND_MACRO(C78E_INTERNAL_SMARTLOG_IMPL(_CORE_, check, C78E_DEBUGBREAK(), "Assertion", "" __VA_OPT__(__VA_ARGS__))))

	#define C78E_VALIDATE(check, escape, ...)		C78E_EXPAND_MACRO(C78E_EXPAND_MACRO(C78E_INTERNAL_SMARTLOG_IMPL(_, check,escape,"Validation","" __VA_OPT__(__VA_ARGS__))))
	#define C78E_CORE_VALIDATE(check, escape, ...)	C78E_EXPAND_MACRO(C78E_EXPAND_MACRO(C78E_INTERNAL_SMARTLOG_IMPL(_CORE_,check,escape,"Validation","" __VA_OPT__(__VA_ARGS__))))

	#define C78E_SOFT_VALIDATE(check, ...)				C78E_EXPAND_MACRO(C78E_EXPAND_MACRO(C78E_INTERNAL_SMARTLOG_IMPL(_,check, , "SoftValidation","" __VA_OPT__(__VA_ARGS__))))
	#define C78E_CORE_SOFT_VALIDATE(check, ...)			C78E_EXPAND_MACRO(C78E_EXPAND_MACRO(C78E_INTERNAL_SMARTLOG_IMPL(_CORE_,check, , "SoftValidation","" __VA_OPT__(__VA_ARGS__))))

	#define C78E_STATIC_ASSERT(check, ...) static_assert((check) __VA_OPT__(, __VA_ARGS__))
	#define C78E_CORE_STATIC_ASSERT(check, ...) static_assert((check) __VA_OPT__(, __VA_ARGS__))
#else
	#define C78E_ASSERT(check, ...)
	#define C78E_CORE_ASSERT(check, ...)

	#define C78E_VALIDATE(check, escape, ...)
	#define C78E_CORE_VALIDATE(check, escape, ...)

	#define C78E_SOFT_VALIDATE(check, ...)
	#define C78E_CORE_SOFT_VALIDATE(check, ...)

	#define C78E_STATIC_ASSERT(check, ...)
	#define C78E_CORE_STATIC_ASSERT(check, ...)
#endif
