#pragma once
#include <C78E/Graphics/Window/Input/MouseCodes.h>
#include "WindowEvent.h"

namespace C78E {

	class MouseMovedEvent : public WindowEvent {
	public:
		MouseMovedEvent(Window& window, const float x, const float y) 
			: WindowEvent(window), m_MouseX(x), m_MouseY(y) { }

		float getX() const { return m_MouseX; }
		float getY() const { return m_MouseY; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryWindow | EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public WindowEvent {
	public:
		MouseScrolledEvent(Window& window, const float xOffset, const float yOffset)
			: WindowEvent(window), m_XOffset(xOffset), m_YOffset(yOffset) {}

		float getXOffset() const { return m_XOffset; }
		float getYOffset() const { return m_YOffset; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryWindow | EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public WindowEvent {
	public:
		Input::MouseCode getMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryWindow | EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(Window& window, const Input::MouseCode button)
			: WindowEvent(window), m_Button(button) {}

		Input::MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(Window& window, const Input::MouseCode button)
			: MouseButtonEvent(window, button) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(Window& window, const Input::MouseCode button)
			: MouseButtonEvent(window, button) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
