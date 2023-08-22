#include "C78ePCH.h"
#include "Platform/Windows/WindowsWindow.h"

#include "C78e/Events/ApplicationEvent.h"
#include "C78e/Events/MouseEvent.h"
#include "C78e/Events/KeyEvent.h"

#include <C78e/Core/Log.h>

#include <glad.h>

namespace C78e {
	
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		C78_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		init(props);
	}

	WindowsWindow::~WindowsWindow() {

		C78_CORE_WARN("destroying winWindow...");
		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		C78_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0) {
			int success = glfwInit();
			C78_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
		#if defined(C78_DEBUG)
			//if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		//
		glfwMakeContextCurrent(m_Window);

		//m_Context = GraphicsContext::Create(m_Window);
		//m_Context->Init();
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		C78_CORE_ASSERT(status, "Failed to init Glad");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		setVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::shutdown() {

		C78_CORE_TRACE("Windows onClose({0} windows closed)", s_GLFWWindowCount);

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::onUpdate() {

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		//m_Context->SwapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::isVSync() const {
		return m_Data.VSync;
	}

}
