#include "C78EPCH.h"

#include "C78E/Core/Log.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad.h>

namespace C78E {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		C78_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		C78_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		C78_CORE_INFO("OpenGL Info:");
		C78_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		C78_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		C78_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		C78_CORE_ASSERT(((GLVersion.major > 4) || ((GLVersion.major == 4) && (GLVersion.minor >= 5))), "C78E requires at least OpenGL version 4.5!");
		
	}

	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}
