#include "C78EPCH.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad.h>

namespace C78E {
	
	void openGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         C78_CORE_FATAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       C78_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          C78_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: C78_CORE_TRACE(message); return;
		}
		
		C78_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::init() {
	#ifdef C78_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openGLMessageCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::enableDepthTest(bool enable) {
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::setDepthFunc(DepthFunc func) {
		glDepthFunc(GL_NEVER + (uint32_t)func);
	}

	void OpenGLRendererAPI::setDepthWrite(bool enable) {
		glDepthMask(enable);
	}

	void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::setLineWidth(float width) {
		glLineWidth(width);
	}

}
