#pragma once

#include "C78e/Core/Core.h"
#include "C78e/Events/Event.h"

namespace C78e {

	struct WindowProps {
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "C78Engine",
			        uint32_t width = 1280,
			        uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};

}
