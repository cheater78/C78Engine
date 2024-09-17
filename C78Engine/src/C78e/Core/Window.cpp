#include "C78EPCH.h"
#include "C78E/Core/Window.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Linux/LinuxWindow.h"

namespace C78E {
	
	Scope<Window> Window::create(const WindowProps& props) {
	#ifdef C78_PLATFORM_WINDOWS
		return createScope<WindowsWindow>(props);
	#elif defined C78_PLATFORM_LINUX
		return createScope<LinuxWindow>(props);
	#else
		C78_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
	
}