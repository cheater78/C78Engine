#pragma once
#ifdef C78E_PLATFORM_GLFW

#include <C78E/Core/Window/Window.h>

namespace C78E {

	class GLFWWindow : public Window {
	public:
		GLFWWindow(const WindowProperties& properties = {}, EventCallbackFunction eventCallbackFunction = nullptr);
		virtual ~GLFWWindow();

		virtual void onUpdate(Timestep delta) override; // opt. Update for Winwow, Layers are handled separately
		virtual void onEvent(Event& e) override; // opt. Events for Winwow, Events are dispatched to Layers separately
		virtual void onDebugRender() override; // opt. Debug for Winwow, Layers Debug Content is handled separately

		// Window attributes
		virtual uint32_t getWidth() const override;
		virtual uint32_t getHeight() const override;
		virtual uvec2 getSize() const override;
		virtual void setSize(const uvec2& size) override;

		virtual void* getNativeWindow() const override;
		virtual WindowProperties getWindowProperties() const override;

		virtual void setWindowMode(WindowMode windowMode) override;
		virtual WindowMode getWindowMode() const override;

		virtual void setRefreshMode(WindowRefreshMode refreshMode) override;
		virtual WindowRefreshMode getRefreshMode() const override;
		virtual bool isRefreshMode(WindowRefreshMode refreshMode) const override;

		virtual WindowMouseCursorMode getMouseMode() const override;
		virtual void setMouseMode(WindowMouseCursorMode mouseMode) override;

		virtual std::string getClipBoardString() const override;
		virtual void setClipboardString(const std::string& str) override;

		virtual void setTitle(const std::string& title) override;
		virtual std::string getTitle() const override;

		// Window Input
		virtual bool isKeyPressed(Input::KeyCode key) override;
		virtual bool isMousePressed(Input::MouseCode mouseButton) override;
		virtual ivec2 getMousePositionFromWindowOriginInPixels()override; // pixels [0,size]
		virtual void setMousePositionFromWindowOriginInPixels(const ivec2& position) override; // pixels [0,size]

	protected:
		GLFWwindow* m_GLFWwindow;
	};

}

#endif // C78E_PLATFORM_GLFW