#pragma once

#include "C78E/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace C78E {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}