#include "C78ePCH.h"
#include "C78e/Core/Window.h"

#ifdef C78_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace C78e {
	
	Scope<Window> Window::create(const WindowProps& props)
	{
	#ifdef C78_PLATFORM_WINDOWS
		return createScope<WindowsWindow>(props);
	#else
		C78_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
	
}