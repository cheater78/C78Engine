#include "C78EPCH.h"
#include "C78E/Core/Window.h"

#ifdef C78_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace C78E {
	
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