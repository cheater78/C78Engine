#include "C78EPCH.h"
#ifdef C78E_PLATFORM_LINUX
#include "LinuxWindow.h"

namespace C78E {
	LinuxWindow::LinuxWindow(const WindowProperties& properties, EventCallbackFunction eventCallbackFunction)
		: GLFWWindow(properties, eventCallbackFunction) { }

	LinuxWindow::~LinuxWindow() { }
}

#endif // C78E_PLATFORM_LINUX