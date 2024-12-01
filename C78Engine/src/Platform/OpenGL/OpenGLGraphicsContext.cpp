#include "C78EPCH.h"
#include "OpenGLGraphicsContext.h"

namespace C78E {

	OpenGLGraphicsContext::OpenGLGraphicsContext(Window& window)
		: m_Window(window) {
	}

	void OpenGLGraphicsContext::init() {
		glfwMakeContextCurrent(getGLFWwindow());
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		C78E_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		C78E_CORE_INFO("OpenGL Info:");
		C78E_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		C78E_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		C78E_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		C78E_CORE_ASSERT(((GLVersion.major > 4) || ((GLVersion.major == 4) && (GLVersion.minor >= 5))), "C78E requires at least OpenGL version 4.5!");
	}

	void OpenGLGraphicsContext::swapBuffers() {
		glfwSwapBuffers(getGLFWwindow());
	}

	GLFWwindow* OpenGLGraphicsContext::getGLFWwindow() const { return static_cast<GLFWwindow*>(m_Window.getNativeWindow()); }

}
