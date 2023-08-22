#pragma once

#include "C78e/Core/Window.h"

#include <GLFW/glfw3.h>

namespace C78e {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;

		unsigned int getWidth() const override { return m_Data.Width; }
		unsigned int getHeight() const override { return m_Data.Height; }

		// Window attributes
		void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		virtual void* getNativeWindow() const { return m_Window; }
	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();
	private:
		GLFWwindow* m_Window;
		//Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}