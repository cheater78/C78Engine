#include "C78EPCH.h"
#ifdef C78E_PLATFORM_WINDOWS
#include "WindowsWindow.h"

namespace C78E {
	WindowsWindow::WindowsWindow(const WindowProperties& properties, EventCallbackFunction eventCallbackFunction)
		: GLFWWindow(properties, eventCallbackFunction) { }

	WindowsWindow::~WindowsWindow() { }
}

#endif // C78E_PLATFORM_WINDOWS