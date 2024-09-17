#pragma once
#ifdef C78_PLATFORM_WINDOWS
#include <Platform/GLFW/GLFWWindow.h>
#include <C78E/Renderer/API/GraphicsContext.h>

namespace C78E {

	class WindowsWindow : public GLFWWindow {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
	};

}

#endif // C78_PLATFORM_WINDOWS