#pragma once

#ifdef C78E_PLATFORM_WINDOWS
	#ifdef C78E_BUILD_DLL
		#define C78E_API __declspec(dllexport)
	#else
		#define C78E_API __declspec(dllimport)
	#endif // C78E_BUILD_DLL
#else
#error Win Only!
#endif // C78E_PLATFORM_WINDOWS
