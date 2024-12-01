#include "C78EPCH.h"
#ifdef C78E_PLATFORM_WINDOWS
#include "WindowsWindow.h"

namespace C78E {
	WindowsWindow::WindowsWindow(const WindowProps& props)
		: GLFWWindow(props) { }

	WindowsWindow::~WindowsWindow() { }
}

#endif // C78E_PLATFORM_WINDOWS