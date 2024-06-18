#include "C78EPCH.h"
#include "OpenGLGraphicsContext.h"

namespace C78E {

	OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle) {
		C78_CORE_ASSERT(m_WindowHandle, "OpenGLGraphicsContext::OpenGLGraphicsContext: m_WindowHandle handle is null!")
	}

	void OpenGLGraphicsContext::init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		C78_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		C78_CORE_INFO("OpenGL Info:");
		C78_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		C78_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		C78_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		C78_CORE_ASSERT(((GLVersion.major > 4) || ((GLVersion.major == 4) && (GLVersion.minor >= 5))), "C78E requires at least OpenGL version 4.5!");
	}

	void OpenGLGraphicsContext::swapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}
