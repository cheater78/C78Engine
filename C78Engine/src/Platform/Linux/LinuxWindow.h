#pragma once
#ifdef C78E_PLATFORM_LINUX
#include <Platform/GLFW/GLFWWindow.h>
#include <C78E/Graphics/API/GraphicsContext.h>

namespace C78E {

	class LinuxWindow : public GLFWWindow {
	public:
		LinuxWindow(const WindowProperties& properties, EventCallbackFunction eventCallbackFunction);
		virtual ~LinuxWindow();
	};

}

#endif // C78E_PLATFORM_LINUX