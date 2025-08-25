#pragma once
#include <C78E/Events/Event.h>
#include "../Window.h"

namespace C78E {

	/**
	 * @brief abstract WindowEvent
	 */
	class WindowEvent : public Event {
	protected:
		WindowEvent(Window& window)
			: Event(), m_Window(window) {
		}
	public:
		EVENT_CLASS_TYPE(Window)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		Window& getWindow() const { return m_Window; }
	protected:
		Window& m_Window;
	};

	class WindowCloseEvent : public WindowEvent {
	public:
		WindowCloseEvent(Window& window)
			: WindowEvent(window) {
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowResizeEvent : public WindowEvent {
	public:
		WindowResizeEvent(Window& window, WindowSize size)
			: WindowEvent(window), m_Size(size) {
		}
		WindowResizeEvent(Window& window, uint32_t width, uint32_t height)
			: WindowEvent(window), m_Size(width, height) {
		}

		WindowSize getSize() const { return m_Size; }
		uint32_t getWidth() const { return m_Size.x; }
		uint32_t getHeight() const { return m_Size.y; }

		std::string toString() const override {
			std::stringstream ss;
			ss << getName() << ": " << m_Size.x << ", " << m_Size.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	private:
		WindowSize m_Size;
	};

	class WindowFocusEvent : public WindowEvent {
	public:
		WindowFocusEvent(Window& window)
			: WindowEvent(window) {
		}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowLostFocusEvent : public WindowEvent {
		public:
		WindowLostFocusEvent(Window& window)
			: WindowEvent(window) {
		}

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowMovedEvent : public WindowEvent {
	public:
		WindowMovedEvent(Window& window, WindowSize size)
			: WindowEvent(window), m_Position(size) {
		}
		WindowMovedEvent(Window& window, int32_t x, int32_t y)
			: WindowEvent(window), m_Position(x, y) {
		}

		int32_t getX() const { return m_Position.x; }
		int32_t getY() const { return m_Position.y; }

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	protected:
		WindowPosition m_Position;
	};
	
}
