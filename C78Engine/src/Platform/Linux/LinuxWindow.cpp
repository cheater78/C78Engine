#include "C78EPCH.h"
#ifdef C78_PLATFORM_LINUX
#include "LinuxWindow.h"

namespace C78E {
	LinuxWindow::LinuxWindow(const WindowProps& props)
		: GLFWWindow(props) { }

	LinuxWindow::~LinuxWindow() { }
}

#endif // C78_PLATFORM_LINUX