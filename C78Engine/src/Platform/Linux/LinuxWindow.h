#pragma once
#ifdef C78_PLATFORM_LINUX
#include <Platform/GLFW/GLFWWindow.h>
#include <C78E/Renderer/API/GraphicsContext.h>

namespace C78E {

	class LinuxWindow : public GLFWWindow {
	public:
		LinuxWindow(const WindowProps& props);
		virtual ~LinuxWindow();
	};

}

#endif // C78_PLATFORM_LINUX