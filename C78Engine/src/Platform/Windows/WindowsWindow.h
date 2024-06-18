#pragma once
#ifdef C78_PLATFORM_WINDOWS

#include "C78E/Core/Window.h"

#include <GLFW/glfw3.h>
#include "Platform/OpenGL/OpenGLContext.h"

namespace C78E {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;

		uint32_t getWidth() const override { return m_Data.width; }
		uint32_t getHeight() const override { return m_Data.height; }

		// Window attributes
		void setEventCallback(const EventCallbackFn& callback) override { m_Data.eventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		virtual MouseMode getMouseMode() override;
		virtual void setMouseMode(MouseMode mouseMode) override;

		virtual void* getNativeWindow() const { return m_Window; }

		virtual WindowProps getWindowProperties() const { return { m_Data.title, m_Data.width, m_Data.height }; }

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData { //TODO: merge with WinProps
			std::string title = "<error>";
			uint32_t width = 100;
			uint32_t height = 100;
			bool vSync = true;

			EventCallbackFn eventCallback;
		};

		WindowData m_Data;

		MouseMode m_MouseMode = MouseMode::NORMAL;
	};

}

#endif // C78_PLATFORM_WINDOWS