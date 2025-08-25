#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Timestep.h>
#include <C78E/Core/Events/Event.h>
#include <C78E/Core/Input/KeyCodes.h>
#include <C78E/Core/Input/MouseCodes.h>
#include <C78E/Core/Window/WindowProperties.h>
#include <C78E/Core/Window/LayerStack.h>
#include <C78E/Core/Window/DebugLayer/DebugLayer.h>
#include <C78E/Math/Math.h>

namespace C78E {

	class GraphicsContext;

	using WindowCount = size_t;
	using WindowID = UUID;
	using WindowSize = Math::uvec2;
	using WindowPosition = Math::ivec2;

	/**
	 * @brief Window base and interface for application windows.
	 */
	class Window {
	public:
		static Scope<Window> create(const WindowProperties& props = {}, EventCallbackFunction eventCallbackFunction = nullptr);
	public:
		Window(const WindowProperties& props = {}, EventCallbackFunction eventCallbackFunction = nullptr);
		virtual ~Window() = default;

		// Layer management
		void pushLayer(Ref<Layer> layer);
		void popLayer(Ref<Layer> layer);
		void pushOverlay(Ref<Layer> layer);

		// Window Handling
		inline bool isRunning() const;
		inline void close();

		template<typename T>
		T* getNativeWindowAs() {
			return (T*)getNativeWindow();
		}

		// Graphics
		GraphicsContext& getGraphicsContext() const;

	public: // Window Tick/Event - called by Application(prob don't wanna call those)
		void callUpdate(Timestep delta);
		void callEvent(Event& e);

	public: // Window Interface
		virtual void onUpdate(Timestep delta) {}; // opt. Update for Winwow, Layers are handled separately
		virtual void onEvent(Event& e) {}; // opt. Events for Winwow, Events are dispatched to Layers separately
		virtual void onDebugRender() {}; // opt. Debug for Winwow, Layers Debug Content is handled separately

		// Window attributes
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uvec2 getSize() const = 0;
		virtual void setSize(const uvec2& size) = 0;
		

		virtual void* getNativeWindow() const = 0;
		virtual WindowProperties getWindowProperties() const = 0;

		virtual void setWindowMode(WindowMode windowMode) = 0;
		virtual WindowMode getWindowMode() const = 0;

		virtual void setRefreshMode(WindowRefreshMode refreshMode) = 0;
		virtual WindowRefreshMode getRefreshMode() const = 0;
		virtual bool isRefreshMode(WindowRefreshMode refreshMode) const = 0;

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
		void setWindowProperties(const WindowProperties& properties);

	private:
		void onWindowCloseEvent(Event& event);
	protected:
		bool m_Running = true;
		WindowProperties m_WindowProperties; // createProperties by default, should be updated by the Window implementation
		EventCallbackFunction m_EventCallback = nullptr; // to be registered in the Window implementation

		LayerStack m_LayerStack;
		Ref<DebugLayer> m_DebugLayer = nullptr; // opt. Layer on top of everything, for debugging purposes

		Scope<GraphicsContext> m_Context;

		//TODO: Windows should run on a separate thread
	};

}
