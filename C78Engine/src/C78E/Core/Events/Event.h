#pragma once
#include <C78E/Core/Core.h>

namespace C78E {

	enum class EventType {
		None = 0,
		ApplicationTick, ApplicationUpdate, ApplicationRender,
		Window, WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryWindow			= BIT(1),
		EventCategoryInput          = BIT(2),
		EventCategoryKeyboard       = BIT(3),
		EventCategoryMouse          = BIT(4),
		EventCategoryMouseButton    = BIT(5)
	};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class Event {
	public:
		virtual ~Event() = default;

		bool handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		bool IsInCategory(EventCategory category) { return getCategoryFlags() & category; }
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event& event) : m_Event(event) { }
		
		// F will be deduced by the compiler
		template<typename T, typename F>
		void dispatch(const F& func) {
			if(m_Event.handled) {
				return;
			}
			// exact match
			if (m_Event.getEventType() == T::getStaticType()) {
				func(static_cast<T&>(m_Event));
			}
			else if (T* e = dynamic_cast<T*>(&m_Event)) {
				func(*e);
			}
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.toString();
	}

	using EventCallbackFunction = std::function<void(Event&)>;

}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(C78E::Event& e) {
		return e.toString();
	}

}

