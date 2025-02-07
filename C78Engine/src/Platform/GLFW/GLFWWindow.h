#pragma once
#ifdef C78E_PLATFORM_GLFW
#include <C78E/Renderer/API/GraphicsContext.h>

namespace C78E {

	class GLFWWindow : public Window {
	public:
		GLFWWindow(const WindowProps& props);
		virtual ~GLFWWindow();

		virtual void onUpdate() override;

		virtual uint32_t getWidth() const override;
		virtual uint32_t getHeight() const override;
		virtual void setResolution(const Resolution& resolution) override;

		virtual void* getNativeWindow() const override;
		virtual WindowProps getWindowProperties() const override;

		virtual void setEventCallback(const EventCallbackFn& callback) override;

		virtual void setWindowMode(WindowMode windowMode) override;
		virtual WindowMode getWindowMode() const override;

		virtual void setRefreshMode(RefreshMode refreshMode) override;
		virtual RefreshMode getRefreshMode() const override;
		virtual bool isRefreshMode(RefreshMode refreshMode) const override;

		virtual MouseMode getMouseMode() const override;
		virtual void setMouseMode(MouseMode mouseMode) override;

		virtual std::string getClipBoardString() const override;
		virtual void setClipboardString(const std::string& str) override;
	protected:
		virtual void init(const WindowProps& props);
		virtual void shutdown();
	protected:
		WindowData m_Data;
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;
		MouseMode m_MouseMode = MouseMode::Normal;

		struct SavedWindowData {
			int width, height;
			int x, y;
		} m_SavedData;
	};

}

#endif // C78E_PLATFORM_GLFW