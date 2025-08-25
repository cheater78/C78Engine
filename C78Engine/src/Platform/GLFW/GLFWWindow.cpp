#include "C78EPCH.h"
#ifdef C78E_PLATFORM_GLFW
#include "GLFWWindow.h"

#include <C78E/Core/Events/ApplicationEvent.h>
#include <C78E/Core/Events/MouseEvent.h>
#include <C78E/Core/Events/KeyEvent.h>

#include <C78E/Graphics/API/GraphicsContext.h>
#include <C78E/Graphics/API/GraphicsInstance.h>
namespace C78E {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		C78E_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	static bool createGLFWwindow(GLFWwindow** window, WindowProperties& properties) {
		if (s_GLFWWindowCount == 0) {
			C78E_CORE_VALIDATE(glfwInit(), return false, "createGLFWwindow: Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);

			// VK
			GraphicsInstance::create(API::Vulkan);
		}
		//TODO: Monitor selection API
		//int monitorCount = 0;
		//GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
		//GLFWmonitor* monitor = monitors[0];
		//const char* name = glfwGetMonitorName(monitor);
		GLFWmonitor* monitor = nullptr;

		switch (properties.windowMode) {
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
			properties.size.x = mode->width;
			properties.size.y = mode->height;
			break;
		}

		// VK
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		*window = glfwCreateWindow((int)properties.size.x, (int)properties.size.y, properties.title.c_str(), monitor, NULL);
		C78E_CORE_VALIDATE(window, return false, "createGLFWwindow: glfwCreateWindow failed!");
		++s_GLFWWindowCount;
		return true;
	}

	static void destroyGLFWwindow(GLFWwindow* window) {
		glfwDestroyWindow(window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0) {
			glfwTerminate();
		}
	}

	GLFWWindow::GLFWWindow(const WindowProperties& properties, EventCallbackFunction eventCallbackFunction)
		: Window(properties, eventCallbackFunction) {
		C78E_CORE_INFO("Creating GLFW window: {}", std::to_string(m_WindowProperties));

		C78E_CORE_VALIDATE(createGLFWwindow(&m_GLFWwindow, m_WindowProperties), return, "GLFWWindow::GLFWWindow: Failed to create underlying GLFWwindow!");

		m_Context = GraphicsContext::create(*this);

		glfwSetWindowUserPointer(m_GLFWwindow, this);

		C78E_CORE_ASSERT(m_EventCallback, "GLFWWindow::GLFWWindow: Event callback function is not set!");
		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_GLFWwindow,
			[](GLFWwindow* window, int width, int height) {
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				win.m_WindowProperties.size.x = static_cast<uint32_t>(width);
				win.m_WindowProperties.size.y = static_cast<uint32_t>(height);

				WindowResizeEvent event(win, win.m_WindowProperties.size);
				win.m_EventCallback(event);
			}
		);

		glfwSetWindowCloseCallback(m_GLFWwindow, 
			[](GLFWwindow* window) {
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event(win);
				win.m_EventCallback(event);
			}
		);
		
		glfwSetKeyCallback(m_GLFWwindow,
			[](GLFWwindow* window, int key, int scancode, int action, int mods) {
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(win, key, false);
					win.m_EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(win, key);
					win.m_EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(win, key, true);
					win.m_EventCallback(event);
					break;
				}
				}
			}
		);

		glfwSetCharCallback(m_GLFWwindow,
			[](GLFWwindow* window, unsigned int keycode) {
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(win, keycode);
				win.m_EventCallback(event);
			}
		);

		glfwSetMouseButtonCallback(m_GLFWwindow,
			[](GLFWwindow* window, int button, int action, int mods) {
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(win, button);
					win.m_EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(win, button);
					win.m_EventCallback(event);
					break;
				}
				}
			}
		);

		glfwSetScrollCallback(m_GLFWwindow,
			[](GLFWwindow* window, double xOffset, double yOffset) {
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event(win, (float)xOffset, (float)yOffset);
				win.m_EventCallback(event);
			}
		);

		glfwSetCursorPosCallback(m_GLFWwindow, 
			[](GLFWwindow* window, double xPos, double yPos) {
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event(win, (float)xPos, (float)yPos);
				win.m_EventCallback(event);
			}
		);

		setWindowProperties(properties);
	}

	GLFWWindow::~GLFWWindow() {
		C78E_CORE_INFO("Destroying GLFW window: {}", std::to_string(m_WindowProperties));
		destroyGLFWwindow(m_GLFWwindow);
	}

	void GLFWWindow::onUpdate(Timestep delta) {
		glfwPollEvents();
	}

	void GLFWWindow::onEvent(Event& e) {
	}

	void GLFWWindow::onDebugRender() {
	}

	// Window attributes

	uint32_t GLFWWindow::getWidth() const {
		return m_WindowProperties.size.x;
	}
	uint32_t GLFWWindow::getHeight() const {
		return m_WindowProperties.size.y;
	}
	glm::uvec2 GLFWWindow::getSize() const {
		return m_WindowProperties.size;
	}
	void GLFWWindow::setSize(const uvec2& size) {
		C78E_CORE_VALIDATE(m_WindowProperties.windowMode != WindowMode::BorderlessWindow, return, "GLFWWindow::setResolution: called in BorderlessWindow mode!");
		glm::ivec2 origin = { 0, 0 };
		glfwGetWindowFrameSize(m_GLFWwindow, &origin.x, &origin.y, nullptr, nullptr);
		if (!origin.x && !origin.y)
			origin = { 64, 64 };
		GLFWmonitor* monitor = (m_WindowProperties.windowMode == WindowMode::Windowed) ? nullptr : glfwGetPrimaryMonitor();
		m_WindowProperties.size = size;
		glfwSetWindowMonitor(
			m_GLFWwindow, 
			monitor, 
			origin.x, origin.y, 
			m_WindowProperties.size.x, m_WindowProperties.size.y, 
			(m_WindowProperties.refreshMode < 2) ? GLFW_DONT_CARE : m_WindowProperties.refreshMode
		);
	}

	void* GLFWWindow::getNativeWindow() const {
		return m_GLFWwindow;
	}
	WindowProperties GLFWWindow::getWindowProperties() const {
		return m_WindowProperties;
	}
	
	void GLFWWindow::setWindowMode(WindowMode windowMode) {
		if (m_WindowProperties.windowMode == windowMode)
			return;
		m_WindowProperties.windowMode = windowMode;
		GLFWmonitor* monitor = nullptr;
		switch (m_WindowProperties.windowMode) {
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
			m_WindowProperties.size.x = mode->width;
			m_WindowProperties.size.y = mode->height;
			break;
		}
		glfwSetWindowMonitor(
			m_GLFWwindow,
			monitor,
			0, 0,
			m_WindowProperties.size.x, m_WindowProperties.size.y,
			(m_WindowProperties.refreshMode < 2) ? GLFW_DONT_CARE : m_WindowProperties.refreshMode
		);
	}

	WindowMode GLFWWindow::getWindowMode() const {
		return m_WindowProperties.windowMode;
	}


	void GLFWWindow::setRefreshMode(WindowRefreshMode refreshMode) {
		m_WindowProperties.refreshMode = refreshMode;
		// glfwSwapInterval(refreshMode); // TODO: how to handle for Vulkan? since its native there
	}

	WindowRefreshMode GLFWWindow::getRefreshMode() const {
		return m_WindowProperties.refreshMode; 
	}
	bool GLFWWindow::isRefreshMode(WindowRefreshMode refreshMode) const {
		return m_WindowProperties.refreshMode == refreshMode;
	}

	WindowMouseCursorMode GLFWWindow::getMouseMode() const {
		int mode = glfwGetInputMode(m_GLFWwindow, GLFW_CURSOR);
		switch (mode) {
		case GLFW_CURSOR_NORMAL:
			return WindowMouseCursorMode::Normal;
		case GLFW_CURSOR_HIDDEN:
			return WindowMouseCursorMode::Hidden;
		case GLFW_CURSOR_DISABLED:
			return WindowMouseCursorMode::Disabled;
		default:
			return WindowMouseCursorMode::Normal;
		}
	}

	void GLFWWindow::setMouseMode(WindowMouseCursorMode mouseMode) {
		int mode = GLFW_CURSOR_NORMAL;
		switch (mouseMode) {
		case C78E::WindowMouseCursorMode::Normal: mode = GLFW_CURSOR_NORMAL; break;
		case C78E::WindowMouseCursorMode::Hidden: mode = GLFW_CURSOR_HIDDEN; break;
		case C78E::WindowMouseCursorMode::Disabled: mode = GLFW_CURSOR_DISABLED; break;
		default: C78E_CORE_ASSERT("GLFWWindow::setMouseMode: Illegal Mouse Input Mode!"); break;
		}
		glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, mode);
	}

	std::string GLFWWindow::getClipBoardString() const {
		return std::string(glfwGetClipboardString(m_GLFWwindow));
	}
	
	void GLFWWindow::setClipboardString(const std::string& str) {
		glfwSetClipboardString(m_GLFWwindow, str.c_str());
	}

	void GLFWWindow::setTitle(const std::string& title) {
		m_WindowProperties.title = title;
		glfwSetWindowTitle(m_GLFWwindow, title.c_str());
	}

	std::string GLFWWindow::getTitle() const {
		//m_WindowProperties.title = std::string(glfwGetWindowTitle(m_GLFWwindow));
		return m_WindowProperties.title;
	}

	// Window Input

	bool GLFWWindow::isKeyPressed(const Input::KeyCode key) {
		auto state = glfwGetKey(m_GLFWwindow, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool GLFWWindow::isMousePressed(const Input::MouseCode button) {
		auto state = glfwGetMouseButton(m_GLFWwindow, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	ivec2 GLFWWindow::getMousePositionFromWindowOriginInPixels() {
		double xpos, ypos;
		glfwGetCursorPos(m_GLFWwindow, &xpos, &ypos);
		return { static_cast<int>(xpos), static_cast<int>(ypos) };
	}

	void GLFWWindow::setMousePositionFromWindowOriginInPixels(const ivec2& position) {
		glfwSetCursorPos(m_GLFWwindow, static_cast<double>(position.x), static_cast<double>(position.y));
	}

}

#endif // C78E_PLATFORM_GLFW
