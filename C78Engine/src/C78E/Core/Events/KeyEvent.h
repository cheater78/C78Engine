#pragma once
#include <C78E/Core/Events/WindowEvent.h>
#include <C78E/Core/Input/KeyCodes.h>


namespace C78E {

	class KeyEvent : public WindowEvent {
	public:
		Input::KeyCode getKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryWindow | EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(Window& window, const Input::KeyCode keycode)
			: WindowEvent(window), m_KeyCode(keycode) { }

		Input::KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(Window& window, const Input::KeyCode keycode, bool isRepeat = false)
			: KeyEvent(window, keycode), m_IsRepeat(isRepeat) { }

		bool isRepeat() const { return m_IsRepeat; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(Window& window, const Input::KeyCode keycode)
			: KeyEvent(window, keycode) { }

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(Window& window, const Input::KeyCode keycode)
			: KeyEvent(window, keycode) { }

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
	
}
