#pragma once
#ifdef C78E_PLATFORM_WINDOWS
#include <Platform/GLFW/GLFWWindow.h>
#include <C78E/Graphics/API/GraphicsContext.h>

namespace C78E {

	class WindowsWindow : public GLFWWindow {
	public:
		WindowsWindow(const WindowProperties& properties, EventCallbackFunction eventCallbackFunction = nullptr);
		virtual ~WindowsWindow();
	};

}

#endif // C78E_PLATFORM_WINDOWS
