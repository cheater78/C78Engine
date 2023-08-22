#pragma once

#include "C78e/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace C78e {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}