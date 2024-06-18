#pragma once
#include <C78E/Renderer/API/GraphicsContext.h>

struct GLFWwindow;

namespace C78E {

	class OpenGLGraphicsContext : public GraphicsContext {
	public:
		OpenGLGraphicsContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}