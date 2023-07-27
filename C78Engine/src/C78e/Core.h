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

