#include "C78EPCH.h"
#include "Window.h"

#include <Platform/Windows/WindowsWindow.h>
#include <Platform/Linux/LinuxWindow.h>

#include <C78E/Core/Events/WindowEvent.h>
#include <C78E/Graphics/API/GraphicsContext.h>

namespace C78E {

	Scope<Window> Window::create(const WindowProperties& props, EventCallbackFunction eventCallbackFunction) {
	#ifdef C78E_PLATFORM_WINDOWS
		return createScope<WindowsWindow>(props, eventCallbackFunction);
	#elif defined C78E_PLATFORM_LINUX
		return createScope<LinuxWindow>(props, eventCallbackFunction);
	#else
		C78E_CORE_STATIC_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}

	Window::Window(const WindowProperties& props, EventCallbackFunction eventCallbackFunction)
		: m_WindowProperties(props), m_EventCallback(eventCallbackFunction) {
	}

	void Window::pushLayer(Ref<Layer> layer) {
		m_LayerStack.pushLayer(layer);
	}
	inline void Window::popLayer(Ref<Layer> layer) {
		m_LayerStack.popLayer(layer);
	}
	inline void Window::pushOverlay(Ref<Layer> layer) {
		m_LayerStack.pushOverlay(layer);
	}

	inline bool Window::isRunning() const {
		return m_Running;
	}
	inline void Window::close() {
		C78E_CORE_VALIDATE(isRunning(), return, "Window::close: Window is not alive!");
		m_Running = false;
	}

	// Graphics
	inline GraphicsContext& Window::getGraphicsContext() const { return *m_Context; }

	/**
	 * @brief triggers an update for the window and all its layers.
	 * Mainly used by the Application to update the window and layers.
	 * You probably don't want to call this manually.
	 * @param delta the Timestep since the last update in seconds.
	 */
	void Window::callUpdate(Timestep delta) {
		C78E_CORE_VALIDATE(isRunning(), return, "Window::callUpdate: Window is not alive!");

		for (Ref<Layer> layer : m_LayerStack) {
			layer->onUpdate(delta);
		}

		if (m_DebugLayer) {
			m_DebugLayer->begin();
			onDebugRender();
			m_DebugLayer->onDebugRender();
			for (Ref<Layer> layer : m_LayerStack) {
				layer->onDebugRender();
			}
			m_DebugLayer->end();
		}

		onUpdate(delta);
		if(m_Context) {
			if (Ref<SwapChain> swapChain = m_Context->getSwapChain()) {
				swapChain->nextFrame();
			}
		}
	}

	/**
	 * @brief Handles an event by dispatching it to the appropriate event handler and propagating it through the layer stack.
	 * @param e The event to be processed and dispatched.
	 */
	void Window::callEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(C78E_BIND_THIS_METHOD(Window::onWindowCloseEvent));
		if (e.handled) {
			return;
		}
		onEvent(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			if (e.handled) {
				break;
			}
			(*--it)->onEvent(e);
		}
	}

	vec2 Window::getMousePositionFromWindowOriginWindowRelative() {
		return vec2(getMousePositionFromWindowOriginInPixels()) / vec2(getSize());
	}

	vec2 Window::getMousePositionFromWindowCenterWindowRelative() {
		const ivec2 mousePosition = getMousePositionFromWindowOriginInPixels();
		const vec2 windowHalfSize = 0.5f * vec2(getSize());
		const vec2 centeredMousePosition = vec2(mousePosition) - windowHalfSize;
		return centeredMousePosition / windowHalfSize;
	}

	void Window::setMousePositionFromWindowOriginWindowRelative(const vec2& position) {
		setMousePositionFromWindowOriginInPixels(ivec2(vec2(getSize()) * position));
	}

	void Window::setMousePositionFromWindowCenterWindowRelative(const vec2& position) {
		setMousePositionFromWindowOriginInPixels(ivec2(0.5f * vec2(getSize()) * position));
	}

	void Window::setResolution(const Resolution& resolution) {
		setSize(Resolution::resolution(resolution));
	}

	void Window::setWindowProperties(const WindowProperties& properties) {
		setTitle(properties.title);
		setSize(properties.size);
		setWindowMode(properties.windowMode);
		setMouseMode(properties.mouseMode);
	}

	void Window::onWindowCloseEvent(Event& event) {
		close();
		event.handled = true;
	}

}
