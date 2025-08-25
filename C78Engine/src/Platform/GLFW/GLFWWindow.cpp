#include "C78EPCH.h"
#ifdef C78E_PLATFORM_GLFW
#include "GLFWWindow.h"
#include "GLFWMonitor.h"

#include <C78E/Core/Application/Events/ApplicationEvent.h>
#include <C78E/Graphics/Window/Events/MouseEvent.h>
#include <C78E/Graphics/Window/Events/KeyEvent.h>

#include <C78E/Graphics/API/GraphicsContext.h>

namespace C78E {

	GLFWWindow::GLFWWindow(const WindowCreateInfo& createInfo)
		: Window() {
		C78E_CORE_INFO("Creating GLFW window");

		Ref<GLFWMonitor> monitor = castRef<GLFWMonitor>(createInfo.monitor);
		if (!monitor) {
			monitor = castRef<GLFWMonitor>(Application::get().getWindowSystem().getPrimaryMonitor());
			C78E_CORE_ASSERT(monitor, "GLFWWindow::GLFWWindow: failed to acquire a valid FullscreenMonitor!");
			m_FullScreenMonitor = monitor;
		}

		GLFWmonitor* nativeMonitor = nullptr; // Windowed or Borderless Monitor = none
		WindowSize size = createInfo.size; // Windowed Size or Fullscreen Res

		if (createInfo.windowMode == WindowMode::FullScreen) { // FullScreen requires a monitor
			nativeMonitor = (GLFWmonitor*)monitor->getNativeMonitor();
		}
		if (createInfo.windowMode == WindowMode::BorderlessWindow) {
			size = monitor->getSize();
		}

		//TODO: for VK.. doesnt belong here
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


		m_GLFWwindow = glfwCreateWindow(size.x, size.y, createInfo.title.c_str(), nativeMonitor, NULL);
		C78E_CORE_VALIDATE(m_GLFWwindow, return, "GLFWWindow::GLFWWindow: glfwCreateWindow failed!");

		//TODO: center Window is probably a feature for Window in general


		m_Context = GraphicsContext::create(*this);

		glfwSetWindowUserPointer(m_GLFWwindow, this);

		m_EventCallback = createInfo.eventCallbackFunction;
		if (!m_EventCallback) {
			m_EventCallback = C78E_BIND_THIS_METHOD(GLFWWindow::callEvent);
		}


		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_GLFWwindow,
			[](GLFWwindow* window, int width, int height) {
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				WindowResizeEvent event(win, WindowSize(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
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

	}

	GLFWWindow::~GLFWWindow() {
		C78E_CORE_INFO("Destroying GLFW window...");
		glfwDestroyWindow(m_GLFWwindow);
	}

	void GLFWWindow::onUpdate(Timestep delta) {
	}

	void GLFWWindow::onEvent(Event& e) {
	}

	void GLFWWindow::onDebugRender() {
	}

	// Window attributes

	WindowPosition GLFWWindow::getPosition() const {
		WindowPosition position;
		MainThread::get().call(
			[&]() -> bool {
				glfwGetWindowPos(m_GLFWwindow, &position.x, &position.y);
				return true;
			}
		);
		return position;
	}

	void GLFWWindow::setPosition(const WindowPosition& position) {
		MainThread::get().call(
			[&]() -> bool {
				glfwSetWindowPos(m_GLFWwindow, position.x, position.y);
				return true;
			}
		);
	}

	WindowSize GLFWWindow::getSize() const {
		int x, y;
		MainThread::get().call(
			[&]() -> bool {
				glfwGetWindowSize(m_GLFWwindow, &x, &y);
				return true;
			}
		);
		return WindowSize(x, y);
	}
	void GLFWWindow::setSize(const WindowSize& size) {
		MainThread::get().call(
			[&]() -> bool {
				glfwSetWindowSize(m_GLFWwindow, size.x, size.y);
				return true;
			}
		);
	}

	void* GLFWWindow::getNativeWindow() const {
		return m_GLFWwindow;
	}
	
	void GLFWWindow::setWindowMode(WindowMode windowMode) {
		const WindowMode currentWindowMode = getWindowMode();
		const WindowMode& targetWindowMode = windowMode;

		if (currentWindowMode == targetWindowMode) {
			return; // Nothing to do
		}

		GLFWmonitor* currentNativeMonitor = glfwGetWindowMonitor(m_GLFWwindow);
		
		WindowPosition targetWindowPosition = { 0, 0 };
		WindowSize targetWindowSize;
		GLFWmonitor* targetNativeMonitor = nullptr;
		
		switch (targetWindowMode) {
		case WindowMode::Windowed:
			if (currentWindowMode == WindowMode::FullScreen) {
				const GLFWMonitor monitor(currentNativeMonitor);
				targetWindowSize = monitor.getSize() / 2u;
				targetWindowPosition = monitor.getSize() / 4u;
				break;
			}
			// BorderlessWindow
			targetWindowSize = getSize() / 2u; // half screen size
			targetWindowPosition = getSize() / 4u; // centered
			break;
		case WindowMode::FullScreen:
			targetNativeMonitor = (GLFWmonitor*)m_FullScreenMonitor->getNativeMonitor();
			targetWindowSize = m_FullScreenMonitor->getSize();
			break;
		case WindowMode::BorderlessWindow:
			if (currentWindowMode == WindowMode::FullScreen) {
				const GLFWMonitor monitor(currentNativeMonitor);
				targetWindowSize = monitor.getSize();
				break;
			}
			// Windowed
			targetWindowSize = m_FullScreenMonitor->getSize();
			break;
		}

		glfwSetWindowMonitor(m_GLFWwindow, targetNativeMonitor, targetWindowPosition.x, targetWindowPosition.y, targetWindowSize.x, targetWindowSize.y, GLFW_DONT_CARE);
	}

	WindowMode GLFWWindow::getWindowMode() const {
		GLFWmonitor* glfwMonitor = glfwGetWindowMonitor(m_GLFWwindow);

		if (glfwMonitor) { // glfwMonitor is set for Fullscreen only
			return WindowMode::FullScreen;
		}
		const bool definetlyWindowed = static_cast<bool>(glfwGetWindowAttrib(m_GLFWwindow, GLFW_DECORATED)); // decoration for windowed only
		if (definetlyWindowed) {
			return WindowMode::Windowed;
		}
		// At this point it could be a undecorated windowed or borderless Fullscreen window
		const WindowSize size = getSize();
		const WindowPosition pos = getPosition();
		const GLFWMonitor monitor(glfwMonitor);
		const MonitorSize monitorSize = monitor.getSize();

		const WindowSize borderlessSize = monitorSize; // Window spans full monitor area
		static const WindowPosition borderlessPosition = { 0, 0 }; // Window is located at origin

		const bool isBorderlessFulscreen = (size == borderlessSize && pos == borderlessPosition);
		if (isBorderlessFulscreen) {
			return WindowMode::BorderlessWindow;
		}
		return WindowMode::Windowed;
	}


	WindowMouseCursorMode GLFWWindow::getMouseMode() const {
		int mode;
		MainThread::get().call(
			[&]() -> bool {
				mode = glfwGetInputMode(m_GLFWwindow, GLFW_CURSOR);
				return true;
			}
		);
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
		case WindowMouseCursorMode::Normal: mode = GLFW_CURSOR_NORMAL; break;
		case WindowMouseCursorMode::Hidden: mode = GLFW_CURSOR_HIDDEN; break;
		case WindowMouseCursorMode::Disabled: mode = GLFW_CURSOR_DISABLED; break;
		default: C78E_CORE_ASSERT("GLFWWindow::setMouseMode: Illegal Mouse Input Mode!"); break;
		}
		MainThread::get().call(
			[&]() -> bool {
				glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, mode);
				return true;
			}
		);
	}

	std::string GLFWWindow::getClipBoardString() const {
		std::string str;
		MainThread::get().call(
			[&]() -> bool {
				str = glfwGetClipboardString(m_GLFWwindow);
				return true;
			}
		);
		return str;
	}
	
	void GLFWWindow::setClipboardString(const std::string& str) {
		MainThread::get().call(
			[&]() -> bool {
				glfwSetClipboardString(m_GLFWwindow, str.c_str());
				return true;
			}
		);
	}

	void GLFWWindow::setTitle(const std::string& title) {
		MainThread::get().call(
			[&]() -> bool {
				glfwSetWindowTitle(m_GLFWwindow, title.c_str());
				return true;
			}
		);
	}

	std::string GLFWWindow::getTitle() const {
		std::string str;
		MainThread::get().call(
			[&]() -> bool {
				str = glfwGetWindowTitle(m_GLFWwindow);
				return true;
			}
		);
		return str;
	}

	// Window Input

	bool GLFWWindow::isKeyPressed(const Input::KeyCode key) {
		int state;
		MainThread::get().call(
			[&]() -> bool {
				state = glfwGetKey(m_GLFWwindow, static_cast<int32_t>(key));
				return true;
			}
		);
		return state == GLFW_PRESS;
	}

	bool GLFWWindow::isMousePressed(const Input::MouseCode button) {
		int state;
		MainThread::get().call(
			[&]() -> bool {
				state = glfwGetMouseButton(m_GLFWwindow, static_cast<int32_t>(button));
				return true;
			}
		);
		return state == GLFW_PRESS;
	}

	ivec2 GLFWWindow::getMousePositionFromWindowOriginInPixels() {
		double xpos, ypos;
		MainThread::get().call(
			[&]() -> bool {
				glfwGetCursorPos(m_GLFWwindow, &xpos, &ypos);
				return true;
			}
		);
		return { static_cast<int>(xpos), static_cast<int>(ypos) };
	}

	void GLFWWindow::setMousePositionFromWindowOriginInPixels(const ivec2& position) {
		MainThread::get().call(
			[&]() -> bool {
				glfwSetCursorPos(m_GLFWwindow, static_cast<double>(position.x), static_cast<double>(position.y));
				return true;
			}
		);
	}

}

#endif // C78E_PLATFORM_GLFW
