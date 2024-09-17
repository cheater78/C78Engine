#include "C78EPCH.h"
#ifdef C78_PLATFORM_WINDOWS
#include "WindowsWindow.h"

namespace C78E {
	WindowsWindow::WindowsWindow(const WindowProps& props)
		: GLFWWindow(props) { }

	WindowsWindow::~WindowsWindow() { }
}

#endif // C78_PLATFORM_WINDOWS