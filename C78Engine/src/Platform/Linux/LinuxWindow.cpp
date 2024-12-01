#include "C78EPCH.h"
#ifdef C78E_PLATFORM_LINUX
#include "LinuxWindow.h"

namespace C78E {
	LinuxWindow::LinuxWindow(const WindowProps& props)
		: GLFWWindow(props) { }

	LinuxWindow::~LinuxWindow() { }
}

#endif // C78E_PLATFORM_LINUX