#pragma once
#include <C78E/Events/Event.h>

namespace C78E {
	
	/**
	 * @brief abstract ApplicationEvent
	 */
	class ApplicationEvent : public Event {
	public:
		ApplicationEvent() = default;
		virtual ~ApplicationEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class ApplicationTickEvent : public ApplicationEvent {
	public:
		ApplicationTickEvent() = default;

		EVENT_CLASS_TYPE(ApplicationTick)
	};

	class ApplicationUpdateEvent : public ApplicationEvent {
	public:
		ApplicationUpdateEvent() = default;

		EVENT_CLASS_TYPE(ApplicationUpdate)
	};

	class ApplicationRenderEvent : public ApplicationEvent {
	public:
		ApplicationRenderEvent() = default;

		EVENT_CLASS_TYPE(ApplicationRender)
	};
}
