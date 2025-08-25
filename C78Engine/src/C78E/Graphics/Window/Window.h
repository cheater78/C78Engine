#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Timestep.h>
#include <C78E/Events/Event.h>
#include <C78E/Math/Math.h>

#include "Layer/LayerStack.h"
#include "Layer/DebugLayer.h"
#include "Monitor.h"
#include "WindowProperties.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

namespace C78E {

	class GraphicsContext;

	using WindowCount = size_t;
	using WindowID = UUID;
	using WindowSize = Math::uvec2;
	using WindowPosition = Math::ivec2;

	struct WindowCreateInfo {
		std::string title = "C78Engine Window";
		uvec2 size = Resolution::resolution(Resolution::FHD);
		WindowMode windowMode = WindowMode::Windowed;
		WindowRefreshMode refreshMode = WindowRefreshMode::Vsync;
		Ref<Monitor> monitor = nullptr; // nullptr will result in the PrimaryMonitor
		EventCallbackFunction eventCallbackFunction = nullptr; // opt. eventCallbackFunction to emit Window events to, else the Windows own callEvent
	};

	/**
	 * @brief Window base and interface for application windows.
	 */
	class Window {
	public:
		static Scope<Window> create(const WindowCreateInfo& createInfo);
	public:
		Window() = default;
		virtual ~Window() = default;

		// Layer management
		void pushLayer(Ref<Layer> layer);
		void popLayer(Ref<Layer> layer);
		void pushOverlay(Ref<Layer> layer);

		// Window Handling
		inline bool isRunning() const;
		inline void close();

		// Graphics
		GraphicsContext& getGraphicsContext() const;

		template<typename T>
		T* getNativeWindowAs() {
			return static_cast<T*>(getNativeWindow());
		}

	public: // Window Tick/Event - called by Application(prob don't wanna call those)
		void callUpdate(Timestep delta);
		void callEvent(Event& e);

	public: // Window Interface
		virtual void onUpdate(Timestep delta) {}; // opt. Update for Winwow, Layers are handled separately
		virtual void onEvent(Event& e) {}; // opt. Events for Winwow, Events are dispatched to Layers separately
		virtual void onDebugRender() {}; // opt. Debug for Winwow, Layers Debug Content is handled separately

		// Window attributes
		virtual WindowPosition getPosition() const = 0;
		virtual void setPosition(const WindowPosition& position) = 0;

		virtual WindowSize getSize() const = 0;
		virtual void setSize(const WindowSize& size) = 0;
		
		virtual void* getNativeWindow() const = 0;

		virtual void setWindowMode(WindowMode windowMode) = 0;
		virtual WindowMode getWindowMode() const = 0;

		virtual WindowMouseCursorMode getMouseMode() const = 0;
		virtual void setMouseMode(WindowMouseCursorMode mouseMode) = 0;

		virtual std::string getClipBoardString() const = 0;
		virtual void setClipboardString(const std::string& str) = 0;

		virtual void setTitle(const std::string& title) = 0;
		virtual std::string getTitle() const = 0;

		// Window Input
		virtual bool isKeyPressed(Input::KeyCode key) = 0;
		virtual bool isMousePressed(Input::MouseCode mouseButton) = 0;

		virtual ivec2 getMousePositionFromWindowOriginInPixels() = 0; // pixels [0,size]
		virtual vec2 getMousePositionFromWindowOriginWindowRelative(); // UV [0,1]
		virtual vec2 getMousePositionFromWindowCenterWindowRelative(); // NDC [-1,1]
		inline vec2 getMousePositionNDC() { return getMousePositionFromWindowCenterWindowRelative(); }

		virtual void setMousePositionFromWindowOriginInPixels(const ivec2& position) = 0; // pixels [0,size]
		virtual void setMousePositionFromWindowOriginWindowRelative(const vec2& position); // UV [0,1]
		virtual void setMousePositionFromWindowCenterWindowRelative(const vec2& position); // NDC [-1,1]
		inline void setMousePositionNDC(const vec2& position) { setMousePositionFromWindowCenterWindowRelative(position); }

	public:
		void setResolution(const Resolution& resolution);

	private:
		void onWindowCloseEvent(Event& event);
	protected:
		bool m_Running = true;
		LayerStack m_LayerStack;
		Ref<DebugLayer> m_DebugLayer = nullptr; // opt. Layer on top of everything, for debugging purposes
		Scope<GraphicsContext> m_Context = nullptr; // opt. GraphicsContext

		Ref<Monitor> m_FullScreenMonitor = nullptr;
	};

}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::WindowCreateInfo& createInfo) {
		return ""; //TODO:
	}

}
