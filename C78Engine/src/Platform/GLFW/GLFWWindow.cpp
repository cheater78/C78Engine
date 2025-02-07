#include "C78EPCH.h"
#ifdef C78E_PLATFORM_GLFW
#include "GLFWWindow.h"

#include <C78E/Renderer/API/RendererAPI.h>

#include <C78E/Events/ApplicationEvent.h>
#include <C78E/Events/MouseEvent.h>
#include <C78E/Events/KeyEvent.h>

namespace C78E {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		C78E_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	GLFWWindow::GLFWWindow(const WindowProps& props) {
		init(props);
	}

	GLFWWindow::~GLFWWindow() {
		C78E_CORE_WARN("Destroying GLFWWindow...");
		shutdown();
	}

	void GLFWWindow::init(const WindowProps& props) {
		m_Data = props;

		C78E_CORE_INFO("Creating window {0} ({1}, {2}) on platform: GLFW", props.title, props.width, props.height);

		if (s_GLFWWindowCount == 0) {
			int success = glfwInit();
			C78E_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
		#if defined(C78E_DEBUG)
			if (RendererAPI::getAPI() == RendererAPI::API::OpenGL) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

			//TODO: Monitor selection API
			//int monitorCount = 0;
			//GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
			//GLFWmonitor* monitor = monitors[0];
			//const char* name = glfwGetMonitorName(monitor);
			GLFWmonitor* monitor = nullptr;

			switch (m_Data.windowMode) {
			case WindowMode::Windowed:
				monitor = nullptr;
				break;
			case WindowMode::FullScreen:
				monitor = glfwGetPrimaryMonitor();
				break;
			case WindowMode::BorderlessWindow:
				monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
				m_Data.width = mode->width;
				m_Data.height = mode->height;
				break;
			}
			m_Window = glfwCreateWindow((int)m_Data.width, (int)m_Data.height, m_Data.title.c_str(), monitor, NULL);
			++s_GLFWWindowCount;
		}


		m_Context = GraphicsContext::create(*this);
		m_Context->init();

		glfwSetWindowUserPointer(m_Window, &m_Data);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
			case GLFW_PRESS: {
				KeyPressedEvent event(key, 0);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent event(key);
				data.eventCallback(event);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent event(key, true);
				data.eventCallback(event);
				break;
			}
			}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			data.eventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent event(button);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent event(button);
				data.eventCallback(event);
				break;
			}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
			});
	}

	void GLFWWindow::shutdown() {
		C78E_CORE_TRACE("Windows onClose({0} windows closed)", s_GLFWWindowCount);

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
			glfwTerminate();
	}

	void GLFWWindow::onUpdate() {
		glfwPollEvents();
		m_Context->swapBuffers();
	}

	uint32_t GLFWWindow::getWidth() const { return m_Data.width; }
	uint32_t GLFWWindow::getHeight() const { return m_Data.height; }

	void GLFWWindow::setResolution(const Resolution& resolution) {
		C78E_CORE_VALIDATE(m_Data.windowMode != WindowMode::BorderlessWindow, return, "GLFWWindow::setResolution: called in BorderlessWindow mode!");
		glm::ivec2 origin = { 0, 0 };
		glfwGetWindowFrameSize(m_Window, &origin.x, &origin.y, nullptr, nullptr);
		if (!origin.x && !origin.y)
			origin = { 64, 64 };
		GLFWmonitor* monitor = (m_Data.windowMode == WindowMode::Windowed) ? nullptr : glfwGetPrimaryMonitor();
		glm::ivec2 res = Resolution::resolution(resolution);
		m_Data.width = res.x;
		m_Data.height = res.y;
		glfwSetWindowMonitor(m_Window, monitor, origin.x, origin.y, res.x, res.y, (m_Data.refreshMode < 2) ? GLFW_DONT_CARE : m_Data.refreshMode);
	}

	void* GLFWWindow::getNativeWindow() const { return m_Window; }
	Window::WindowProps GLFWWindow::getWindowProperties() const { return m_Data; }
	void GLFWWindow::setEventCallback(const EventCallbackFn& callback) { m_Data.eventCallback = callback; }
	
	void GLFWWindow::setWindowMode(WindowMode windowMode) {
		if (m_Data.windowMode == windowMode)
			return;
		m_Data.windowMode = windowMode;
		GLFWmonitor* monitor = nullptr;
		switch (m_Data.windowMode) {
		case WindowMode::Windowed:
			monitor = nullptr;
			break;
		case WindowMode::FullScreen:
			monitor = glfwGetPrimaryMonitor();
			break;
		case WindowMode::BorderlessWindow:
			monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			m_Data.width = mode->width;
			m_Data.height = mode->height;
			break;
		}
		glfwSetWindowMonitor(m_Window, monitor, 0, 0, m_Data.width, m_Data.height, (m_Data.refreshMode < 2) ? GLFW_DONT_CARE : m_Data.refreshMode);
	}

	Window::WindowMode GLFWWindow::getWindowMode() const {
		return m_Data.windowMode;
	}


	void GLFWWindow::setRefreshMode(RefreshMode refreshMode) {
		m_Data.refreshMode = refreshMode;
		glfwSwapInterval(refreshMode);
	}

	Window::RefreshMode GLFWWindow::getRefreshMode() const { return m_Data.refreshMode; }
	bool GLFWWindow::isRefreshMode(RefreshMode refreshMode) const { return m_Data.refreshMode == refreshMode; }

	Window::MouseMode GLFWWindow::getMouseMode() const { return m_MouseMode; }

	void GLFWWindow::setMouseMode(MouseMode mouseMode) {
		m_MouseMode = mouseMode;
		int mode = GLFW_CURSOR_NORMAL;
		switch (mouseMode) {
		case C78E::Window::Normal: mode = GLFW_CURSOR_NORMAL; break;
		case C78E::Window::Hidden: mode = GLFW_CURSOR_HIDDEN; break;
		case C78E::Window::Disabled: mode = GLFW_CURSOR_DISABLED; break;
		default: C78E_CORE_ASSERT("GLFWWindow::setMouseMode: Illegal Mouse Input Mode!"); break;
		}
		glfwSetInputMode(m_Window, GLFW_CURSOR, mode);
	}

	std::string GLFWWindow::getClipBoardString() const {
		return std::string(glfwGetClipboardString(m_Window));
	}
	
	void GLFWWindow::setClipboardString(const std::string& str) {
		glfwSetClipboardString(m_Window, str.c_str());
	}

}

#endif // C78E_PLATFORM_GLFW